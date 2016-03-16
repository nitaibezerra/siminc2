<?php

/**
 * Classe responsavel pela manipulacao do historico da previsao de termino da demanda fabrica
 * @author Silas Matheus
 * @name HistoricoPrevisaoTermino
 *
 */
class HistoricoPrevisaoTermino extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.historicoprevisaotermino";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hptid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'hptid'       => null,
        'obsid'       => null,
        'prevtermino' => null
    );
    private $dataRedefinicao;
    private $previsaoTermino;
    private $ultimaDataPrevisaoTerminoEmDia;

    public function setDataRedefinicao( DateTime $dataRedefinicao ) {
        $this->dataRedefinicao = $dataRedefinicao;
    }

    public function getDataRedefinicao() {
        return $this->dataRedefinicao;
    }

    public function setPrevisaoTermino( DateTime $previsaoTermino ) {
        $this->previsaoTermino = $previsaoTermino;
    }

    public function getPrevisaoTermino() {
        return $this->previsaoTermino;
    }

    public function setUltimaDataPrevisaoTerminoEmDia( DateTime $ultimaDataPrevisaoTerminoEmDia ) {
        $this->ultimaDataPrevisaoTerminoEmDia = $ultimaDataPrevisaoTerminoEmDia;
    }

    public function getUltimaDataPrevisaoTerminoEmDia() {
        return $this->ultimaDataPrevisaoTerminoEmDia;
    }

    /**
     * Monta a listagem do historico
     * @param <int> $odsid
     * @return void
     */
    public function montaGridSS( $odsid = '', $scsid = '' ) {

        if ( empty( $scsid ) ) {

            $sqlSS = "SELECT scsid FROM fabrica.ordemservico WHERE odsid=" . $odsid;
            $scsid = $this->pegaUm( $sqlSS );
        }

        $sql = "SELECT
					(to_char(obs.obsdata, 'DD/MM/YYYY')) as data_inclusao,
					(to_char(hpt.prevtermino, 'DD/MM/YYYY')) as data,
					(SELECT distinct usu.usunome FROM seguranca.usuario usu 
			INNER JOIN fabrica.observacoes obs2 on obs2.usucpf = usu.usucpf
			where usu.usucpf = obs.usucpf) as usuario, 
					(obs.obsdsc) as dsc,
					(ed.esddsc) as situacao
				FROM
					$this->stNomeTabela hpt
				INNER JOIN
					fabrica.observacoes obs
					ON obs.obsid=hpt.obsid
				INNER JOIN
					workflow.estadodocumento ed
					ON ed.esdid = obs.obssituacao
				WHERE
					obs.scsid=$scsid
				ORDER BY
					hpt.hptid DESC";

        $cabecalho = array( 'Data cadastro', 'Data término', 'Autor', 'Justificativa', 'Situação SS' );
        $alinhameto = array( 'center', 'left' );
        return $this->monta_lista( $sql, $cabecalho, 20, 50, false, "center", 'S', '', '', $alinhameto );
    }

    public function montaGridOS( $odsid = '', $scsid = '' ) {

        if ( empty( $scsid ) ) {

            $sqlSS = "SELECT scsid FROM fabrica.ordemservico WHERE odsid=" . $odsid;
            $scsid = $this->pegaUm( $sqlSS );
        }

        $sql = "SELECT
					(to_char(obs.obsdata, 'DD/MM/YYYY')) as data_inclusao,
					(to_char(hpt.prevtermino, 'DD/MM/YYYY')) as data,
					(SELECT distinct usu.usunome FROM seguranca.usuario usu 
			INNER JOIN fabrica.observacoes obs2 on obs2.usucpf = usu.usucpf
			where usu.usucpf = obs.usucpf) as usuario, 
					(obs.obsdsc) as dsc,
					(ed.esddsc) as situacao
				FROM
					$this->stNomeTabela hpt
				INNER JOIN
					fabrica.observacoes obs
					ON obs.obsid=hpt.obsid
				LEFT JOIN
					workflow.estadodocumento ed
					ON ed.esdid = obs.obssituacao
				WHERE
					obs.odsid=$odsid
				ORDER BY
					hpt.hptid DESC";

        $cabecalho = array( 'Data cadastro', 'Data término', 'Autor', 'Justificativa', 'Situação SS' );
        $alinhameto = array( 'center', 'left' );
        return $this->monta_lista( $sql, $cabecalho, 20, 50, false, "center", 'S', '', '', $alinhameto );
    }

    /**
     * Retorna a data de previsão de término inicial
     * 
     * @param int $odsid 
     */
    public function retornarDataPrevisaoTerminoInicialEmpresaItem1( $odsid ) {

        $sql = "SELECT os.odsid , os.odsdtprevinicio , os.odsdtprevtermino , historico.prevterminoinicial,
						tramitacaoexecucao.datatramitacao, datatermino.prevtermino::date as data_termino_planejada,
						(CASE 
							WHEN 
								( ed.esdid NOT IN ( " . WF_ESTADO_OS_VERIFICACAO . "," . WF_ESTADO_OS_APROVACAO . "," . WF_ESTADO_OS_ATESTO_TECNICO . ", " . WF_ESTADO_OS_FINALIZADA . ") OR ed.esdid IS NULL ) 
								AND to_char( CURRENT_DATE::date,'YYYY-MM-DD') > to_char(COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								AND situacao.hpstipo = 'I' 
								AND to_char( situacao.datapausa::date,'YYYY-MM-DD') <= to_char(COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								THEN 'OS EM DIA'			
							WHEN 
								ed.esdid IN ( " . WF_ESTADO_OS_VERIFICACAO . "," . WF_ESTADO_OS_APROVACAO . "," . WF_ESTADO_OS_ATESTO_TECNICO . ", " . WF_ESTADO_OS_FINALIZADA . ") 
								AND to_char(tramitacaoexecucao.datatramitacao::date,'YYYY-MM-DD') <= to_char(COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								THEN 'OS EM DIA'
							WHEN 
								ed.esdid IN ( " . WF_ESTADO_OS_VERIFICACAO . "," . WF_ESTADO_OS_APROVACAO . "," . WF_ESTADO_OS_ATESTO_TECNICO . ", " . WF_ESTADO_OS_FINALIZADA . ")
								AND to_char(tramitacaoexecucao.datatramitacao::date,'YYYY-MM-DD') > to_char( COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								THEN 'OS EM ATRASO'
							WHEN 
								( ed.esdid NOT IN ( " . WF_ESTADO_OS_VERIFICACAO . "," . WF_ESTADO_OS_APROVACAO . "," . WF_ESTADO_OS_ATESTO_TECNICO . ", " . WF_ESTADO_OS_FINALIZADA . ") OR ed.esdid IS NULL ) 
								AND to_char( CURRENT_DATE::date,'YYYY-MM-DD') > to_char(COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								THEN 'OS EM ATRASO'
							WHEN 
								( ed.esdid NOT IN ( " . WF_ESTADO_OS_VERIFICACAO . "," . WF_ESTADO_OS_APROVACAO . "," . WF_ESTADO_OS_ATESTO_TECNICO . ", " . WF_ESTADO_OS_FINALIZADA . ") OR ed.esdid IS NULL ) 
								AND to_char( CURRENT_DATE::date,'YYYY-MM-DD') <= to_char(COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								THEN 'OS EM DIA'   
							 END)  as status_os, 

							ed.esddsc,ed.esdid,

						--COALESCE( tramitacaoexecucao.datatramitacao, current_date)::date - COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date as dias_em_atraso,
						
						COALESCE( tramitacaoexecucao.datatramitacao, current_date)::date - COALESCE( datatermino.prevtermino::date, os.odsdtprevtermino)::date as dias_em_atraso,

						COALESCE( historico.prevterminoinicial, os.odsdtprevtermino )::date  - os.odsdtprevinicio::date + 1 as dias_previstos
					FROM fabrica.ordemservico os	
					LEFT JOIN fabrica.tipoordemservico tos 
						ON tos.tosid = os.tosid
					LEFT JOIN fabrica.solicitacaoservico ss 
						ON ss.scsid = os.scsid
					LEFT JOIN workflow.documento d 
						ON d.docid = os.docid
					LEFT JOIN workflow.estadodocumento ed 
						ON ed.esdid = d.esdid

					LEFT JOIN 
						( 
							SELECT obs.scsid,  hps.hpstipo , obs.obsdata as datapausa
							FROM fabrica.historicopausaservico hps
							INNER JOIN fabrica.observacoes obs
								ON hps.obsid = obs.obsid
							INNER JOIN 
								(
									SELECT obs.scsid, MAX( hps.hpsid ) AS id
									FROM fabrica.historicopausaservico hps
									INNER JOIN fabrica.observacoes obs
										ON hps.obsid = obs.obsid 
									GROUP BY obs.scsid 
								) as historico
								ON  hps.hpsid = historico.id
							ORDER BY obs.scsid 
						) AS situacao
						ON os.scsid = situacao.scsid
					LEFT JOIN
						(
							SELECT obs.odsid,  hpt.prevtermino as prevterminoinicial, hpt.hptid, obs.obsdata as dataalteracao
							FROM fabrica.historicoprevisaotermino hpt
							INNER JOIN fabrica.observacoes obs
								ON hpt.obsid = obs.obsid
							INNER JOIN 
								( 
									SELECT obs.odsid, MIN( hpt.hptid ) AS id
									FROM fabrica.historicoprevisaotermino hpt
									INNER JOIN fabrica.observacoes obs
										ON hpt.obsid = obs.obsid 
									GROUP BY obs.odsid 
								) AS historico
								ON  hpt.hptid = historico.id
						) AS historico
						ON os.odsid = historico.odsid

					LEFT JOIN 
						(
							SELECT os.odsid, MAX(hd.htddata) as datatramitacao		
							FROM workflow.historicodocumento hd
							INNER JOIN workflow.acaoestadodoc ac 
								ON ac.aedid = hd.aedid
							INNER JOIN workflow.estadodocumento ed 
								ON ed.esdid = ac.esdidorigem
							INNER JOIN fabrica.ordemservico os
								ON hd.docid = os.docid
							WHERE ac.esdiddestino = " . WF_ESTADO_OS_VERIFICACAO . "
							GROUP BY os.odsid
							ORDER BY os.odsid
						) as tramitacaoexecucao
						ON os.odsid = tramitacaoexecucao.odsid
					LEFT JOIN 
					(
						SELECT oss.odsid, hpt.prevtermino
						FROM fabrica.ordemservico oss
						INNER JOIN fabrica.observacoes obs
							ON oss.odsid = obs.odsid
						INNER JOIN fabrica.historicoprevisaotermino hpt
							ON obs.obsid = hpt.obsid

						INNER JOIN 
						(
						SELECT oss.odsid,  MAX ( hpt.hptid ) as hptid
						FROM fabrica.ordemservico oss
						INNER JOIN fabrica.observacoes obs
							ON oss.odsid = obs.odsid
						INNER JOIN fabrica.historicoprevisaotermino hpt
							ON obs.obsid = hpt.obsid
						INNER JOIN 
						(
							SELECT osss.odsid, hpt.hptid, hpt.prevtermino as data
							FROM fabrica.ordemservico osss
							INNER JOIN fabrica.observacoes obs
								ON osss.odsid = obs.odsid
							INNER JOIN fabrica.historicoprevisaotermino hpt
								ON obs.obsid = hpt.obsid

						) as termino
						ON oss.odsid = termino.odsid
					WHERE obs.obsdata <= termino.data
					GROUP BY oss.odsid

					) as teste1
					ON hpt.hptid = teste1.hptid
					) as datatermino	
						ON os.odsid = datatermino.odsid

					WHERE os.odsid = {$odsid}";

        $retorno                            = $this->pegaLinha( $sql );
        $retorno['data_termino_planejada']  = $this->recupereUltimaDataPrevisaoTerminoEmDia( $odsid )->format( 'd/m/Y' );
        $retorno['dias_em_atraso']          = $this->getDiasEmAtraso( $odsid );
        $retorno['datatramitacao']          = $this->empresaItem1PossuiDataEntrega( $odsid ) ? $this->recupereDataEntregaEmpresaItem1( $odsid )->format( 'd/m/Y' ) : '';
        $retorno['status_os']               = 'OS EM DIA';
        $retorno['dias_previstos']          = $this->recupereDiasPrevistosOS( $odsid );

        if ( !empty( $retorno['data_termino_planejada'] ) && empty( $retorno['datatramitacao'] ) ) {
            $data                 = DateTimeUtil::retiraMascara( $retorno['data_termino_planejada'] );
            $timestampDataTermino = strtotime( $data );
            $timestampDataAtual   = strtotime( date( 'Y/m/d' ) );

            $diferencaTimestamp = $timestampDataTermino - $timestampDataAtual;
            if ( $diferencaTimestamp < 0 ) {
                $retorno['status_os'] = 'OS EM ATRASO';
            }
        }

        if ( !empty( $retorno['datatramitacao'] ) ) {
            $timestampDataEntrega = strtotime( DateTimeUtil::retiraMascara( $retorno['datatramitacao'] ) );
            $data                 = DateTimeUtil::retiraMascara( $retorno['data_termino_planejada'] );
            $timestampDataTermino = strtotime( $data );
            $diferencaTimestamp   = $timestampDataTermino - $timestampDataEntrega;

            if ( $diferencaTimestamp < 0 ) {
                $retorno['status_os'] = 'OS EM ATRASO';
            }
        }

        return $retorno;
    }

    public function isOSEntregueEmDia( $odsid ) {
        //Existem casos que a data é a mesma e a hora da ultimaDataPrevisaoTerminoEmDia é zerada.
        //Por isso será desconsiderada as horas
        $ultimaPrevisaoTerminoEmDia       = $this->recupereUltimaDataPrevisaoTerminoEmDia( $odsid );
        $stringUltimaPrevisaoTerminoEmDia = $ultimaPrevisaoTerminoEmDia->format( 'Y-m-d' ) . " 00:00:00";
        $ultimaPrevisaoTerminoEmDia       = new DateTime( $stringUltimaPrevisaoTerminoEmDia );


        $ordemServico = new OrdemServico();
        $ordemServico = $ordemServico->recuperePorId( $odsid );

        if ( $ordemServico->isOsEmpresaItem1() ) {
            
        } else {
            if ( $this->isRetornadaParaAguardandoContagemEmpresaItem2( $odsid ) ) {
                $dataEntrega       = $this->recupereDataEntregaRetornadaEmpresaItem2( $odsid );
                $stringDataEntrega = $dataEntrega->format( 'Y-m-d' ) . " 00:00:00";
                $dataEntrega       = new DateTime( $stringDataEntrega );
            } else {
                $dataEntrega       = $this->recupereDataEntregaEmpresaItem2( $odsid );
                $stringDataEntrega = $dataEntrega->format( 'Y-m-d' ) . " 00:00:00";
                $dataEntrega       = new DateTime( $stringDataEntrega );
            }
        }

        $emDia = $dataEntrega <= $ultimaPrevisaoTerminoEmDia;
        return $emDia;
    }

    public function recupereUltimaDataPrevisaoTerminoEmDia( $odsid ) {
        $sql = "SELECT
				hptid, obs.obsdata as data_redefinicao, hpt.prevtermino as ultima_previsao_termino
				FROM fabrica.historicoprevisaotermino hpt
				join fabrica.observacoes obs on hpt.obsid = obs.obsid
				where obs.odsid = $odsid 
				ORDER BY hptid ASC";

        $resultSet = $this->carregar( $sql );

        if ( !empty( $resultSet ) ) {
            foreach ( $resultSet as $linha ) {

                $dataRedefinicao       = new DateTime( $linha['data_redefinicao'] );
                $stringDataRedefinicao = $dataRedefinicao->format( 'Y-m-d' ) . " 00:00:00";
                $dataRedefinicao       = new DateTime( $stringDataRedefinicao );

                $previsaoTermino       = new DateTime( $linha['ultima_previsao_termino'] );
                $stringPrevisaoTermino = $previsaoTermino->format( 'Y-m-d' ) . " 00:00:00";
                $previsaoTermino       = new DateTime( $stringPrevisaoTermino );

                if ( $this->getDataRedefinicao() == null && $this->getPrevisaoTermino() == null ) {
                    $this->setDataRedefinicao( $dataRedefinicao );
                    $this->setPrevisaoTermino( $previsaoTermino );
                    $this->setUltimaDataPrevisaoTerminoEmDia( $previsaoTermino );
                } else {
                    if ( $dataRedefinicao <= $this->getUltimaDataPrevisaoTerminoEmDia() ) {
                        $this->setPrevisaoTermino( $previsaoTermino );
                        $this->setUltimaDataPrevisaoTerminoEmDia( $previsaoTermino );
                        continue;
                    } else {
                        break;
                    }
                    $this->setPrevisaoTermino( $previsaoTermino );
                }
            }
            return $this->getUltimaDataPrevisaoTerminoEmDia();
        } else {
            $ordemServico = new OrdemServico();
            $ordemServico = $ordemServico->recuperePorId( $odsid );
            return $ordemServico->getDataPrevisaoDeTermino();
        }
    }

    public function empresaItem1PossuiDataEntrega( $odsid ) {
        return $this->recupereDataEntregaEmpresaItem1( $odsid ) != null;
    }

    public function empresaItem2PossuiDataEntrega( $odsid ) {
// 		if ($this->isRetornadaParaAguardandoContagemEmpresaItem2($odsid)){
// 			return false;
// 		}
        return $this->recupereDataEntregaEmpresaItem2( $odsid ) != null;
    }

    public function recupereDataEntregaEmpresaItem1( $odsid ) {
        $sql = "SELECT MAX(hd.htddata) as datatramitacao				
				FROM workflow.historicodocumento hd
				INNER JOIN workflow.acaoestadodoc ac
					   ON ac.aedid = hd.aedid
				INNER JOIN workflow.estadodocumento ed
					   ON ed.esdid = ac.esdidorigem
				INNER JOIN fabrica.ordemservico os
					   ON hd.docid = os.docid
				WHERE ac.esdiddestino = " . WF_ESTADO_OS_VERIFICACAO . " 
                AND os.odsid = $odsid
                AND ac.esdidorigem = ". WF_ESTADO_OS_EXECUCAO;

        if ( $this->pegaUm( $sql ) == "" ) {
            return null;
        } else {
            return new DateTime( $this->pegaUm( $sql ) );
        }
    }

    public function recupereDataEntregaEmpresaItem2( $odsid ) {
        $sql = "SELECT MAX(hd.htddata) as datatramitacao
				FROM workflow.historicodocumento hd
				INNER JOIN workflow.acaoestadodoc ac
				ON ac.aedid = hd.aedid
				INNER JOIN workflow.estadodocumento ed
				ON ed.esdid = ac.esdidorigem
				INNER JOIN fabrica.ordemservico os
				ON hd.docid = os.docidpf
				WHERE ac.esdidorigem = 273 and ac.esdiddestino	= 275 and os.odsid = $odsid";
        if ( $this->pegaUm( $sql ) == "" ) {
            return null;
        } else {
            return new DateTime( $this->pegaUm( $sql ) );
        }
    }

    public function recupereDataEntregaRetornadaEmpresaItem2( $odsid ) {
        $sql = "SELECT MAX(hd.htddata) as datatramitacao
				FROM workflow.historicodocumento hd
				INNER JOIN workflow.acaoestadodoc ac
				ON ac.aedid = hd.aedid
				INNER JOIN workflow.estadodocumento ed
				ON ed.esdid = ac.esdidorigem
				INNER JOIN fabrica.ordemservico os
				ON hd.docid = os.docidpf
				WHERE ac.esdidorigem = 275 and ac.esdiddestino	= 273 and os.odsid = $odsid";

        if ( $this->pegaUm( $sql ) == "" ) {
            return null;
        } else {
            return new DateTime( $this->pegaUm( $sql ) );
        }
    }

    public function isRetornadaParaAguardandoContagemEmpresaItem2( $odsid ) {
        return $this->recupereDataEntregaRetornadaEmpresaItem2( $odsid ) != null;
    }

    public function getDiasEmAtraso( $odsid ) {
        $ordemServico = new OrdemServico();
        $ordemServico = $ordemServico->recuperePorId( $odsid );

        $dataAtual = new DateTime();

        if ( $ordemServico->isOsEmpresaItem1() ) {

            if ( $ordemServico->getStatusOrdemServico() == StatusOrdemServico::PENDENTE ||
                    $ordemServico->getStatusOrdemServico() == StatusOrdemServico::EM_EXECUCAO ||
                    $ordemServico->getStatusOrdemServico() == "" ) {

                $ultimaDataPrevisaoTerminoEmDia = $this->recupereUltimaDataPrevisaoTerminoEmDia( $odsid );
                return floor( ($dataAtual->format( 'U' ) - $ultimaDataPrevisaoTerminoEmDia->format( 'U' )) / (60 * 60 * 24) );
            } else {
                $ultimaDataPrevisaoTerminoEmDia = $this->recupereUltimaDataPrevisaoTerminoEmDia( $odsid );

                if ( $this->empresaItem1PossuiDataEntrega( $odsid ) ) {
                    $dataTramitacao = $this->recupereDataEntregaEmpresaItem1( $odsid );
                    return floor( ($dataTramitacao->format( 'U' ) - $ultimaDataPrevisaoTerminoEmDia->format( 'U' ) ) / (60 * 60 * 24) );
                }

                return floor( ($dataAtual->format( 'U' ) - $ultimaDataPrevisaoTerminoEmDia->format( 'U' ) ) / (60 * 60 * 24) );
            }
        } else {

            if ( $ordemServico->getStatusOrdemServico() == 273 ||
                    $ordemServico->getStatusOrdemServico() == "" ) {

                $ultimaDataPrevisaoTerminoEmDia = $this->recupereUltimaDataPrevisaoTerminoEmDia( $odsid );
                return floor( ($dataAtual->format( 'U' ) - $ultimaDataPrevisaoTerminoEmDia->format( 'U' )) / (60 * 60 * 24) );
            } else {
                $ultimaDataPrevisaoTerminoEmDia = $this->recupereUltimaDataPrevisaoTerminoEmDia( $odsid );

                if ( $this->empresaItem2PossuiDataEntrega( $odsid ) ) {
                    $dataTramitacao = $this->recupereDataEntregaEmpresaItem2( $odsid );
                    return floor( ($dataTramitacao->format( 'U' ) - $ultimaDataPrevisaoTerminoEmDia->format( 'U' ) ) / (60 * 60 * 24) );
                }

                return floor( ($dataAtual->format( 'U' ) - $ultimaDataPrevisaoTerminoEmDia->format( 'U' ) ) / (60 * 60 * 24) );
            }
        }
    }

    public function retornarDataPrevisaoTerminoInicialEmpresaItem2( $odsid ) {

        $sql = "SELECT os.odsid , os.odsdtprevinicio , os.odsdtprevtermino , historico.prevterminoinicial,
						tramitacaoexecucao.datatramitacao, datatermino.prevtermino::date as data_termino_planejada, 
						(CASE 
							WHEN 
								( ed.esdid NOT IN ( " . WF_ESTADO_CPF_AVALIACAO . "," . WF_ESTADO_CPF_APROVACAO . "," . WF_ESTADO_CPF_REVISAO . ", " . WF_ESTADO_CPF_FINALIZADA . ") OR ed.esdid IS NULL ) 
								AND to_char( CURRENT_DATE::date,'YYYY-MM-DD') > to_char(COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								AND situacao.hpstipo = 'I' 
								AND to_char( situacao.datapausa::date,'YYYY-MM-DD') <= to_char(COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								THEN 'OS EM DIA'			
							WHEN 
								ed.esdid IN ( " . WF_ESTADO_CPF_AVALIACAO . "," . WF_ESTADO_CPF_APROVACAO . "," . WF_ESTADO_CPF_REVISAO . ", " . WF_ESTADO_CPF_FINALIZADA . ")
								AND to_char(tramitacaoexecucao.datatramitacao::date,'YYYY-MM-DD') <= to_char(COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								THEN 'OS EM DIA'
							WHEN 
								ed.esdid IN ( " . WF_ESTADO_CPF_AVALIACAO . "," . WF_ESTADO_CPF_APROVACAO . "," . WF_ESTADO_CPF_REVISAO . ", " . WF_ESTADO_CPF_FINALIZADA . ")
								AND to_char(tramitacaoexecucao.datatramitacao::date,'YYYY-MM-DD') > to_char( COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								THEN 'OS EM ATRASO'
							WHEN 
								( ed.esdid NOT IN ( " . WF_ESTADO_CPF_AVALIACAO . "," . WF_ESTADO_CPF_APROVACAO . "," . WF_ESTADO_CPF_REVISAO . ", " . WF_ESTADO_CPF_FINALIZADA . ")
									OR ed.esdid IS NULL 
								) 
								AND to_char( CURRENT_DATE::date,'YYYY-MM-DD') > to_char(COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								THEN 'OS EM ATRASO'
							WHEN 
								( 
									ed.esdid NOT IN ( " . WF_ESTADO_CPF_AVALIACAO . "," . WF_ESTADO_CPF_APROVACAO . "," . WF_ESTADO_CPF_REVISAO . ", " . WF_ESTADO_CPF_FINALIZADA . ")
									OR ed.esdid IS NULL 
								) 
								AND to_char( CURRENT_DATE::date,'YYYY-MM-DD') <= to_char(COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date,'YYYY-MM-DD')  
								THEN 'OS EM DIA'   
							 END)  as status_os, 

							ed.esddsc,ed.esdid,

						--COALESCE( tramitacaoexecucao.datatramitacao, current_date)::date - COALESCE(historico.prevterminoinicial, os.odsdtprevtermino)::date as dias_em_atraso,
						
						COALESCE( tramitacaoexecucao.datatramitacao, current_date)::date - COALESCE( datatermino.prevtermino::date, os.odsdtprevtermino)::date as dias_em_atraso,

						COALESCE( historico.prevterminoinicial, os.odsdtprevtermino )::date  - os.odsdtprevinicio::date + 1 as dias_previstos
					FROM fabrica.ordemservico os	
					LEFT JOIN fabrica.tipoordemservico tos 
						ON tos.tosid = os.tosid
					LEFT JOIN fabrica.solicitacaoservico ss 
						ON ss.scsid = os.scsid
					LEFT JOIN workflow.documento d 
						ON d.docid = os.docidpf
					LEFT JOIN workflow.estadodocumento ed 
						ON ed.esdid = d.esdid

					LEFT JOIN 
						( 
							SELECT obs.scsid,  hps.hpstipo , obs.obsdata as datapausa
							FROM fabrica.historicopausaservico hps
							INNER JOIN fabrica.observacoes obs
								ON hps.obsid = obs.obsid
							INNER JOIN 
								(
									SELECT obs.scsid, MAX( hps.hpsid ) AS id
									FROM fabrica.historicopausaservico hps
									INNER JOIN fabrica.observacoes obs
										ON hps.obsid = obs.obsid 
									GROUP BY obs.scsid 
								) as historico
								ON  hps.hpsid = historico.id
							ORDER BY obs.scsid 
						) AS situacao
						ON os.scsid = situacao.scsid
					LEFT JOIN
						(
							SELECT obs.odsid,  hpt.prevtermino as prevterminoinicial, hpt.hptid, obs.obsdata as dataalteracao
							FROM fabrica.historicoprevisaotermino hpt
							INNER JOIN fabrica.observacoes obs
								ON hpt.obsid = obs.obsid
							INNER JOIN 
								( 
									SELECT obs.odsid, MIN( hpt.hptid ) AS id
									FROM fabrica.historicoprevisaotermino hpt
									INNER JOIN fabrica.observacoes obs
										ON hpt.obsid = obs.obsid 
									GROUP BY obs.odsid 
								) AS historico
								ON  hpt.hptid = historico.id
						) AS historico
						ON os.odsid = historico.odsid

					LEFT JOIN 
						(
							SELECT os.odsid, MAX(hd.htddata) as datatramitacao		
							FROM workflow.historicodocumento hd
							INNER JOIN workflow.acaoestadodoc ac 
								ON ac.aedid = hd.aedid
							INNER JOIN workflow.estadodocumento ed 
								ON ed.esdid = ac.esdidorigem
							INNER JOIN fabrica.ordemservico os
								ON hd.docid = os.docidpf

							WHERE ac.esdiddestino	= " . WF_ESTADO_CPF_APROVACAO . "
							GROUP BY os.odsid
							ORDER BY os.odsid
						) as tramitacaoexecucao
						ON os.odsid = tramitacaoexecucao.odsid
					 
					LEFT JOIN 
					(
						SELECT oss.odsid, hpt.prevtermino
						FROM fabrica.ordemservico oss
						INNER JOIN fabrica.observacoes obs
							ON oss.odsid = obs.odsid
						INNER JOIN fabrica.historicoprevisaotermino hpt
							ON obs.obsid = hpt.obsid

						INNER JOIN 
						(
						SELECT oss.odsid,  MAX ( hpt.hptid ) as hptid
						FROM fabrica.ordemservico oss
						INNER JOIN fabrica.observacoes obs
							ON oss.odsid = obs.odsid
						INNER JOIN fabrica.historicoprevisaotermino hpt
							ON obs.obsid = hpt.obsid
						INNER JOIN 
						(
							SELECT osss.odsid, hpt.hptid, hpt.prevtermino as data
							FROM fabrica.ordemservico osss
							INNER JOIN fabrica.observacoes obs
								ON osss.odsid = obs.odsid
							INNER JOIN fabrica.historicoprevisaotermino hpt
								ON obs.obsid = hpt.obsid

						) as termino
						ON oss.odsid = termino.odsid
					WHERE obs.obsdata <= termino.data
					GROUP BY oss.odsid

					) as teste1
					ON hpt.hptid = teste1.hptid
					) as datatermino	
						ON os.odsid = datatermino.odsid
					WHERE os.odsid = {$odsid}";

        $ordemServico                      = new OrdemServico();
        $ordemServico->commit();
        $ordemServico                      = $ordemServico->recuperePorId( $odsid );
        $retorno                           = $this->pegaLinha( $sql );
        $diasEmAtraso                      = $this->getDiasEmAtraso( $odsid );
        $retorno['data_termino_planejada'] = $this->recupereUltimaDataPrevisaoTerminoEmDia( $odsid ) == null ? "" : $this->recupereUltimaDataPrevisaoTerminoEmDia( $odsid )->format( 'd/m/Y' );
        $retorno['datatramitacao']         = $this->empresaItem2PossuiDataEntrega( $odsid ) ? $this->recupereDataEntregaEmpresaItem2( $odsid )->format( 'd/m/Y' ) : '';
        $retorno['status_os']              = 'OS EM DIA';
        $retorno['dias_em_atraso']         = $diasEmAtraso > 0 ? $diasEmAtraso : 0;
        $retorno['dias_previstos']         = $this->recupereDiasPrevistosOS( $odsid );

        if ( $this->empresaItem2PossuiDataEntrega( $odsid ) ) {
            $retorno['status_os']      = $this->isOSEntregueEmDia( $odsid ) ? 'OS EM DIA' : 'OS EM ATRASO';
        }

        if ( !empty( $retorno['data_termino_planejada'] ) && empty( $retorno['datatramitacao'] ) ) {
            $data                 = DateTimeUtil::retiraMascara( $retorno['data_termino_planejada'] );
            $timestampDataTermino = strtotime( $data );
            $timestampDataAtual   = strtotime( date( 'Y/m/d' ) );

            $diferencaTimestamp = $timestampDataTermino - $timestampDataAtual;
            if ( $diferencaTimestamp < 0 ) {
                $retorno['status_os'] = 'OS EM ATRASO';
            }
        }

        if ( !empty( $retorno['datatramitacao'] ) ) {
            $timestampDataEntrega = strtotime( DateTimeUtil::retiraMascara( $retorno['datatramitacao'] ) );
            $data                 = DateTimeUtil::retiraMascara( $retorno['data_termino_planejada'] );
            $timestampDataTermino = strtotime( $data );
            $diferencaTimestamp   = $timestampDataTermino - $timestampDataEntrega;

            if ( $diferencaTimestamp < 0 ) {
                $retorno['status_os'] = 'OS EM ATRASO';
            }
        }

        return $retorno;
    }

    /**
     * Verifica se uma OS já foi replanejada. 
     * Alterada data de previsão de término
     * @return bool
     */
    public function isOSReplanejada( $odsid ) {
        $sql = "SELECT os.odsid, count( historico.odsid ) as total
				FROM fabrica.ordemservico os
				LEFT JOIN 
						( 
						SELECT obs.odsid,  hpt.prevtermino, hpt.hptid
						FROM fabrica.historicoprevisaotermino hpt
						INNER JOIN fabrica.observacoes obs
								ON hpt.obsid = obs.obsid
						INNER JOIN 
								( 
								SELECT obs.odsid, MIN( hpt.hptid ) as id
								FROM fabrica.historicoprevisaotermino hpt
								INNER JOIN fabrica.observacoes obs
										ON hpt.obsid = obs.obsid 
								GROUP BY obs.odsid 
								) as historico
								ON  hpt.hptid = historico.id
						) as historico
						ON os.odsid = historico.odsid
				WHERE os.odsid = $odsid
				GROUP BY os.odsid";

        $retorno = $this->pegaLinha( $sql );

        return (bool) $retorno['total'];
    }

    /**
     * Retorna a quantidade de dias previstos para execução de um OS
     * Compreende desde o período de início da OS até a data de previsão 
     * de término ou a última redefinição válida para a empresa do item 2
     * 
     * @param int $odsid - Código da ordem de serviço
     * @return int - Quantidade de dias previstos 
     */
    public function recupereDiasPrevistosOS( $odsid ) {
        $ordemServico       = new OrdemServico();
        $ordemServico       = $ordemServico->recuperePorId( $odsid );
        $dataInicialDemanda = $ordemServico->getDataPrevisaoDeInicio( $odsid );
        $dataFinalDemanda   = $this->recupereUltimaDataPrevisaoTerminoEmDia( $odsid );
        if ( $dataFinalDemanda == null ) {
            $dataFinalDemanda = new DateTime();
        }

        $timestampDataInicial = strtotime( $dataInicialDemanda->format( 'Y/m/d' ) );
        $timestampDataFinal   = strtotime( $dataFinalDemanda->format( 'Y/m/d' ) );
        $diasPrevistos        = (int) floor( ($timestampDataFinal - $timestampDataInicial ) / (60 * 60 * 24) );

        return $diasPrevistos == 0 ? 1 : $diasPrevistos;
    }

    /**
     * Salva nova data de previsao de termino da empresa do item2
     * @param <array> $arrDados - Dados postados
     * @return int
     */
    public function alterarPrevisaoTerminoEmpresaItem2( $arrDados ) {
        
        //var_dump($arrDados);
        //exit;

        if ( empty( $arrDados['scsid'] ) ) {
            $sqlSS = "SELECT scsid FROM fabrica.ordemservico WHERE odsid=" . $arrDados['odsid'];
            $scsid = $this->pegaUm( $sqlSS );
        } else {
            $scsid = $arrDados['scsid'];
        }

        $odsid = $arrDados['odsid'];


        $sqlSituacaoSS = "select ed.esdid from fabrica.solicitacaoservico ss
								join workflow.documento doc on ss.docid = doc.docid
								join workflow.estadodocumento ed on doc.esdid = ed.esdid where scsid=" . $scsid;

        $situacaoSS = $this->pegaUm( $sqlSituacaoSS );

        $sqlHis = "SELECT
							COUNT(*)
					   FROM
							$this->stNomeTabela hpt
					   INNER JOIN
							fabrica.observacoes obs
							ON obs.obsid=hpt.obsid
					   WHERE odsid=" . $odsid;
        
        //die( $sqlHis );

        //recupera quantidade de historico de para a ordem de servico
        $qtd = $this->pegaUm( $sqlHis );
        
        $data        = isset( $arrDados['data_tramite'] ) ?  $arrDados['data_tramite']  : date( 'Y-m-d H:i:s' );

        //entra aqui quando nao tem nenhum registro na tabela de historico previsao termino para poder gravar o primeiro registro com a data inicial
        if ( !$qtd ) {
            $usucpf                 = $_SESSION['usucpf'];
            $sqlDataAnteriorOsItem2 = "SELECT odsdtprevtermino FROM fabrica.ordemservico WHERE odsid = " . $odsid;


            //recupera a previsao de termino final da os item2
            $ansprevtermino = $this->pegaUm( $sqlDataAnteriorOsItem2 );


            $sqlOBS = "INSERT INTO fabrica.observacoes
								(usucpf, odsid, obsdsc, obsstatus, scsid, obsdata, obstp, obssituacao)
						   VALUES
								('$usucpf', $odsid, 'Data Inicial - Fiscal Técnico', 'A', $scsid, '$data', 'J', $situacaoSS)
						   RETURNING
								obsid";

            //FAZ o insert e recupera a id da Observacao
            $obsid = $this->pegaUm( $sqlOBS );

            $sqlHistorico = "INSERT INTO $this->stNomeTabela
								(obsid, prevtermino)
						   VALUES
								($obsid, '$ansprevtermino')
						   RETURNING
								hptid";

            //FAZ o insert em historico previsao termino e recupera o id do HitoricoPrevisaoTermino
            $hptid = $this->pegaUm( $sqlHistorico );
        }
        
        $dataPrevisaoTermino = !empty( $arrDados['prevtermino'] ) ? $arrDados['prevtermino'] : $_POST['prevtermino'];

        $usucpf      = $_SESSION['usucpf'];
        $obsdsc      = trim( $arrDados['obsdsc'] );
        
        $prevtermino = formata_data_sql( $dataPrevisaoTermino );

        $sqlDataAnterior  = "SELECT odsdtprevtermino FROM fabrica.ordemservico WHERE scsid=" . $scsid;
        //var_dump($sqlDataAnterior);exit;
                
        $odsdtprevtermino = $this->pegaUm( $sqlDataAnterior );

//							$sqlSituacaoOS =	"select ed.esdid from fabrica.ordemservico os
//								join workflow.documento doc on os.docidpf = doc.docid
//								join workflow.estadodocumento ed on doc.esdid = ed.esdid where odsid = " . $odsid;
//			
//			$situacaoOS = $this->pegaUm($sqlSituacaoOS);

        $sqlSituacaoSS = "select ed.esdid from fabrica.solicitacaoservico ss
								join workflow.documento doc on ss.docid = doc.docid
								join workflow.estadodocumento ed on doc.esdid = ed.esdid where scsid=" . $scsid;

        $situacaoSS = $this->pegaUm( $sqlSituacaoSS );

        if ( $odsdtprevtermino ) {
            $sqlOBS = "INSERT INTO fabrica.observacoes
									(usucpf, odsid, obsdsc, obsstatus, scsid, obsdata, obstp, obssituacao)
							   VALUES
									('$usucpf', $odsid, '$obsdsc', 'A', $scsid, '$data', 'J', $situacaoSS)
							   RETURNING
									obsid";
            $obsid  = $this->pegaUm( $sqlOBS );

            $sqlHistorico = "INSERT INTO $this->stNomeTabela
									(obsid, prevtermino)
							   VALUES
									($obsid, '$prevtermino')
							   RETURNING
									hptid";

            $hptid = $this->pegaUm( $sqlHistorico );
        }

        $sqlOS = "UPDATE fabrica.ordemservico SET odsdtprevtermino='$prevtermino' WHERE odsid=$odsid RETURNING odsid";
        $odsid = $this->pegaUm( $sqlOS );
        $this->commit();
/*         echo '<pre>';
        var_dump($sqlDataAnterior);
        var_dump($sqlOS);
        var_dump($prevtermino);
        exit; */
        
        //die($sqlOS);
        return $odsid;
    }

    /**
     * Salva nova data de previsao de termino
     * @param <array> $arrDados - Dados postados
     * @return int
     */
    public function salvar( $arrDados ) {

        //recupera o scsid
        if ( empty( $arrDados['scsid'] ) ) {
            $sqlSS = "SELECT scsid FROM fabrica.ordemservico WHERE odsid=" . $arrDados['odsid'];
            $scsid = $this->pegaUm( $sqlSS );
        } else {
            $scsid = $arrDados['scsid'];
        }

        //recupera o odsid
        if ( empty( $arrDados['odsid'] ) ) {
            $odsid = "null";
        } else {
            $odsid = $arrDados['odsid'];
        }
        
        $dataTermino =  $_POST['prevtermino'] ? $_POST['prevtermino'] : $arrDados['prevtermino'];

        $usucpf      = $_SESSION['usucpf'];
        $obsdsc      = trim( $arrDados['obsdsc'] );
        $data        = isset( $arrDados['data_tramite'] ) ? $arrDados['data_tramite']  :  date( 'Y-m-d H:i:s' );
        $prevtermino = formata_data_sql( $dataTermino );

        // grava o primeiro prazo estabelecido pelos fiscais
        if ( $scsid ) {

            $sqlHis = "SELECT
							COUNT(*)
					   FROM
							$this->stNomeTabela hpt
					   INNER JOIN
							fabrica.observacoes obs
							ON obs.obsid=hpt.obsid
					   WHERE scsid=" . $scsid;

            //recupera a quantidade de ocorrencias no historico para a SS
            $qtd = $this->pegaUm( $sqlHis );

            $sqlSituacaoSS = "select ed.esdid from fabrica.solicitacaoservico ss
								join workflow.documento doc on ss.docid = doc.docid
								join workflow.estadodocumento ed on doc.esdid = ed.esdid where scsid=" . $scsid;

            $situacaoSS = $this->pegaUm( $sqlSituacaoSS );

            //entra aqui quando nao tem nenhum registro na tabela de historico previsao termino para poder gravar o primeiro registro com a data inicial
            if ( !$qtd ) {

                $sqlDataAnterior = "SELECT ansprevtermino FROM fabrica.analisesolicitacao WHERE scsid=" . $scsid;


                //recupera a previsao de termino final da analise solicitacao
                $ansprevtermino = $this->pegaUm( $sqlDataAnterior );


                $sqlOBS = "INSERT INTO fabrica.observacoes
								(usucpf, odsid, obsdsc, obsstatus, scsid, obsdata, obstp, obssituacao)
						   VALUES
								('$usucpf', $odsid, 'Data Inicial - Fiscal Técnico', 'A', $scsid, '$data', 'J', $situacaoSS)
						   RETURNING
								obsid";

                //FAZ o insert e recupera a id da Observacao
                $obsid = $this->pegaUm( $sqlOBS );

                $sqlHistorico = "INSERT INTO $this->stNomeTabela
								(obsid, prevtermino)
						   VALUES
								($obsid, '$ansprevtermino')
						   RETURNING
								hptid";

                //FAZ o insert em historico previsao termino e recupera o id do HitoricoPrevisaoTermino
                $hptid = $this->pegaUm( $sqlHistorico );
            }
        }

        // grava o primeiro prazo estabelecido pela fabrica
        if ( $odsid ) {
            $sqlHis = "SELECT
							COUNT(*)
					   FROM
							$this->stNomeTabela hpt
					   INNER JOIN
							fabrica.observacoes obs
							ON obs.obsid=hpt.obsid
					   WHERE odsid=" . $odsid;

            //recupera quantidade de historico de para a ordem de servico
            $qtd = $this->pegaUm( $sqlHis );

//			$sqlSituacaoOS =	"select ed.esdid from fabrica.ordemservico os
//								join workflow.documento doc on os.docid = doc.docid
//								join workflow.estadodocumento ed on doc.esdid = ed.esdid where odsid = " . $odsid;
//			
//			$situacaoOS = $this->pegaUm($sqlSituacaoOS);
//			if ($situacaoOS == ""){
//				$situacaoOS = "null";	
//			}

            $sqlSituacaoSS = "select ed.esdid from fabrica.solicitacaoservico ss
								join workflow.documento doc on ss.docid = doc.docid
								join workflow.estadodocumento ed on doc.esdid = ed.esdid where scsid=" . $scsid;

            $situacaoSS = $this->pegaUm( $sqlSituacaoSS );

            if ( !$qtd ) {

                $sqlDataAnterior = "SELECT odsdtprevtermino FROM fabrica.ordemservico WHERE scsid=" . $scsid;

                //recupera data de previsao de atendimento de uma OS
                $odsdtprevtermino = $this->pegaUm( $sqlDataAnterior );


                if ( $odsdtprevtermino ) {

                    $sqlOBS = "INSERT INTO fabrica.observacoes
									(usucpf, odsid, obsdsc, obsstatus, scsid, obsdata, obstp, obssituacao)
							   VALUES
									('$usucpf', $odsid, 'Data Inicial - Fábrica', 'A', $scsid, '$data', 'J', $situacaoSS)
							   RETURNING
									obsid";

                    //insere uma observação e recupera o id deste insert
                    $obsid = $this->pegaUm( $sqlOBS );

                    $sqlHistorico = "INSERT INTO $this->stNomeTabela
									(obsid, prevtermino)
							   VALUES
									($obsid, '$odsdtprevtermino')
							   RETURNING
									hptid";

                    //insere uma ocorrencia no historico previsao termino para recuperando o seu id
                    $hptid = $this->pegaUm( $sqlHistorico );
                }
            }
        }


//		if ($odsid == "null"){
//			$situacaoSSOS = $situacaoSS;
//		} else {
//			$situacaoSSOS = $situacaoOS;
//		}

        $sqlOBS = "INSERT INTO fabrica.observacoes 
						(usucpf, odsid, obsdsc, obsstatus, scsid, obsdata, obstp, obssituacao)
				   VALUES
						('$usucpf', $odsid, '$obsdsc', 'A', $scsid, '$data', 'J', $situacaoSS)
				   RETURNING
						obsid";

        $obsid = $this->pegaUm( $sqlOBS );

        $sqlHistorico = "INSERT INTO $this->stNomeTabela
						(obsid, prevtermino)
				   VALUES
						($obsid, '$prevtermino')
				   RETURNING
						hptid";

        if ( $odsid ) {
            $sqlOS = "UPDATE fabrica.ordemservico SET odsdtprevtermino='$prevtermino' WHERE odsid=$odsid RETURNING odsid";
            $odsid = $this->pegaUm( $sqlOS );
        }

        if ( $scsid ) {
            $sqlANS = "UPDATE fabrica.analisesolicitacao SET ansprevtermino='$prevtermino' WHERE scsid=$scsid RETURNING ansid";
            $ansid  = $this->pegaUm( $sqlANS );
        }

        return $hptid = $this->pegaUm( $sqlHistorico );
    }

}
