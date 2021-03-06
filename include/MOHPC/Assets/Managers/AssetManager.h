#pragma once

#include "Manager.h"
#include "../Asset.h"
#include "../../Common/Object.h"
#include "../../Utility/SharedPtr.h"
#include "../../Utility/WeakPtr.h"

#include <morfuse/Container/set.h>
#include <typeinfo>
#include <typeindex>
#include <exception>

namespace MOHPC
{
	class AssetManager : public std::enable_shared_from_this<AssetManager>
	{
		friend class Class;

		MOHPC_ASSET_OBJECT_DECLARATION(AssetManager);

	private:
		MOHPC_ASSETS_EXPORTS AssetManager();
		~AssetManager();

	public:
		/** Get the virtual file manager associated with the asset manager. */
		MOHPC_ASSETS_EXPORTS FileManager* GetFileManager() const;

		/** Return a manager class check the *Managers* folder. */
		template<class T>
		SharedPtr<T> GetManager()
		{
			static_assert(std::is_base_of<Manager, T>::value, "T must be a subclass of Manager");

			const std::type_index ti = typeid(T);
			SharedPtr<T> manager = staticPointerCast<T>(GetManager(ti));
			if (manager == nullptr)
			{
				manager = T::create();
				AddManager(ti, manager);
			}
			return manager;
		}

		/** 
		 * Load an asset from disk or pak files.
		 *
		 * @param	Filename	Virtual path to file.
		 * @return	Shared pointer to asset if found, NULL otherwise.
		 * @note	If the asset is already loaded, it is returned from cache.
		 */
		template<class T>
		SharedPtr<T> LoadAsset(const char *Filename)
		{
			SharedPtr<T> A = staticPointerCast<T>(CacheFindAsset(Filename));
			if (!A)
			{
				A = T::create();
				if (!CacheLoadAsset(Filename, A)) {
					return nullptr;
				}
			}
			return A;
		}

	private:
		MOHPC_ASSETS_EXPORTS void AddManager(const std::type_index& ti, const SharedPtr<Manager>& manager);
		MOHPC_ASSETS_EXPORTS SharedPtr<Manager> GetManager(const std::type_index& ti) const;
		MOHPC_ASSETS_EXPORTS SharedPtr<Asset> CacheFindAsset(const char *Filename);
		MOHPC_ASSETS_EXPORTS bool CacheLoadAsset(const char *Filename, const SharedPtr<Asset>& A);

	private:
		mutable FileManager* FM;

		/**
		 * Shared pointer, because each manager is a storage, part of the asset manager
		 * and must be destroyed during or after AssetManager destruction.
		 */
		mfuse::con::set<std::type_index, SharedPtr<Manager>> m_managers;

		/**
		 * Weak pointer, because it's just a cache of loaded assets.
		 * They can be destroyed anytime.
		 */
		mfuse::con::set<str, WeakPtr<Asset>> m_assetCache;
	};
	using AssetManagerPtr = SharedPtr<AssetManager>;

	template<class T>
	SharedPtr<T> AssetObject::GetManager() const
	{
		const SharedPtr<AssetManager> manPtr = GetAssetManager();
		if (manPtr)
		{
			// return the owning asset manager
			return manPtr->GetManager<T>();
		}

		return nullptr;
	}

	namespace AssetError
	{
		class Base : public std::exception {};

		class AssetNotFound : public Base
		{
		public:
			AssetNotFound(const str& inFileName);

			MOHPC_ASSETS_EXPORTS const char* getFileName() const;

		public:
			const char* what() const noexcept override;

		private:
			str fileName;
		};

		/**
		 * The asset file has wrong header.
		 */
		class BadHeader4 : public Base
		{
		public:
			BadHeader4(const uint8_t foundHeader[4], const uint8_t expectedHeader[4]);

			MOHPC_ASSETS_EXPORTS const uint8_t* getHeader() const;
			MOHPC_ASSETS_EXPORTS const uint8_t* getExpectedHeader() const;

		public:
			const char* what() const noexcept override;

		private:
			uint8_t foundHeader[4];
			uint8_t expectedHeader[4];
		};
	}
}
