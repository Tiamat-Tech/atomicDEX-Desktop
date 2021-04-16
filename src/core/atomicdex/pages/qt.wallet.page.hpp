#pragma once

//! Qt
#include <QJsonObject>
#include <QObject>
#include <QVariant>

#include "atomicdex/models/qt.wallet.transactions.model.hpp"

namespace atomic_dex
{
    class wallet_page final : public QObject, public ag::ecs::pre_update_system<wallet_page>
    {
        // Q_Object definition
        Q_OBJECT
        
        using t_qt_synchronized_json   = boost::synchronized_value<QJsonObject>;
        using t_qt_synchronized_string = boost::synchronized_value<QString>;

      public:
        explicit wallet_page(entt::registry& registry, ag::ecs::system_manager& system_manager, QObject* parent = nullptr);
        void update()  override;
        ~wallet_page()  final = default;

        void refresh_ticker_infos() ;
        
        void on_tx_fetch_finished(const tx_fetch_finished&);

        // Getters/Setters
        [[nodiscard]] transactions_model* get_transactions_mdl() const ;
        [[nodiscard]] QString  get_current_ticker() const ;
        void                   set_current_ticker(const QString& ticker) ;
        [[nodiscard]] QVariant get_ticker_infos() const ;
        [[nodiscard]] bool     is_broadcast_busy() const ;
        void                   set_broadcast_busy(bool status) ;
        [[nodiscard]] bool     is_send_busy() const ;
        void                   set_send_busy(bool status) ;
        [[nodiscard]] bool     is_rpc_claiming_busy() const ;
        void                   set_claiming_is_busy(bool status) ;
        [[nodiscard]] bool     is_claiming_faucet_busy() const ;
        void                   set_claiming_faucet_is_busy(bool status) ;
        [[nodiscard]] QVariant get_rpc_claiming_data() const ;
        void                   set_rpc_claiming_data(QVariant rpc_data) ;
        [[nodiscard]] QVariant get_rpc_claiming_faucet_data() const ;
        void                   set_rpc_claiming_faucet_data(QVariant rpc_data) ;
        [[nodiscard]] QString  get_rpc_broadcast_data() const ;
        void                   set_rpc_broadcast_data(QString rpc_data) ;
        [[nodiscard]] QVariant get_rpc_send_data() const ;
        void                   set_rpc_send_data(QVariant rpc_data) ;
        [[nodiscard]] bool     is_tx_fetching_busy() const ;
        void                   set_tx_fetching_busy(bool status) ;
        [[nodiscard]] bool     has_auth_succeeded() const ;
        void                   set_auth_succeeded() ;
        
        // QML API
        Q_INVOKABLE void claim_rewards();
        Q_INVOKABLE void claim_faucet();
        Q_INVOKABLE void broadcast(const QString& tx_hex, bool is_claiming, bool is_max, const QString& amount) ;
        void broadcast_on_auth_finished(bool is_auth, const QString& tx_hex, bool is_claiming, bool is_max, const QString& amount); // Broadcast requires OS local user credentials verification. This is called by the Q_INVOKABLE broadcast() method after entering credentials.
        Q_INVOKABLE void send(const QString& address, const QString& amount, bool max, bool with_fees, QVariantMap fees_data);
        
        // QML API Properties
        Q_PROPERTY(transactions_model* transactions_mdl READ get_transactions_mdl NOTIFY transactionsMdlChanged)
        Q_PROPERTY(QString ticker READ get_current_ticker WRITE set_current_ticker NOTIFY currentTickerChanged)
        Q_PROPERTY(QVariant ticker_infos READ get_ticker_infos NOTIFY tickerInfosChanged)
        Q_PROPERTY(bool is_claiming_busy READ is_rpc_claiming_busy WRITE set_claiming_is_busy NOTIFY rpcClaimingStatusChanged)
        Q_PROPERTY(QVariant claiming_rpc_data READ get_rpc_claiming_data WRITE set_rpc_claiming_data NOTIFY claimingRpcDataChanged)
        Q_PROPERTY(bool is_claiming_faucet_busy READ is_claiming_faucet_busy WRITE set_claiming_faucet_is_busy NOTIFY claimingFaucetStatusChanged)
        Q_PROPERTY(QVariant claiming_faucet_rpc_data READ get_rpc_claiming_faucet_data WRITE set_rpc_claiming_faucet_data NOTIFY claimingFaucetRpcDataChanged)
        Q_PROPERTY(bool is_broadcast_busy READ is_broadcast_busy WRITE set_broadcast_busy NOTIFY broadCastStatusChanged)
        Q_PROPERTY(QString broadcast_rpc_data READ get_rpc_broadcast_data WRITE set_rpc_broadcast_data NOTIFY broadcastDataChanged)
        Q_PROPERTY(bool is_send_busy READ is_send_busy WRITE set_send_busy NOTIFY sendStatusChanged)
        Q_PROPERTY(QVariant send_rpc_data READ get_rpc_send_data WRITE set_rpc_send_data NOTIFY sendDataChanged)
        Q_PROPERTY(bool tx_fetching_busy READ is_tx_fetching_busy WRITE set_tx_fetching_busy NOTIFY txFetchingStatusChanged)
        Q_PROPERTY(bool auth_succeeded READ has_auth_succeeded NOTIFY auth_succeededChanged)
        
        // QML API Properties Signals
      signals:
        void currentTickerChanged();
        void tickerInfosChanged();
        void rpcClaimingStatusChanged();
        void claimingRpcDataChanged();
        void claimingFaucetStatusChanged();
        void claimingFaucetRpcDataChanged();
        void broadCastStatusChanged();
        void broadcastDataChanged();
        void sendStatusChanged();
        void sendDataChanged();
        void transactionsMdlChanged();
        void txFetchingStatusChanged();
        void auth_succeededChanged();

      private:
        ag::ecs::system_manager& m_system_manager;
        transactions_model*      m_transactions_mdl;
        std::atomic_bool         m_is_claiming_busy{false};
        std::atomic_bool         m_is_claiming_faucet_busy{false};
        std::atomic_bool         m_is_broadcast_busy{false};
        std::atomic_bool         m_is_send_busy{false};
        std::atomic_bool         m_tx_fetching_busy{false};
        t_qt_synchronized_json   m_claiming_rpc_result;
        t_qt_synchronized_json   m_claiming_rpc_faucet_result;
        t_qt_synchronized_json   m_send_rpc_result;
        t_qt_synchronized_string m_broadcast_rpc_result;
        bool                     m_auth_succeeded;
    };
} // namespace atomic_dex

REFL_AUTO(type(atomic_dex::wallet_page))