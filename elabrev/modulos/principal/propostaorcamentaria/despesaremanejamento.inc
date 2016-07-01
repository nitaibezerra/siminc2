<?php
	
	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
		Módulo: despesadetalhamento.inc
		Finalidade: Manutenção na tabela despesaacao.
	*/
	
	/*
		Vários itens de despesa podem ser administrados nesta tela, porém
		apenas as de uma determinada ação. Os parâmetros para essa tela	pode
		ser o unicod, prgcod, acacod e loccod ou somente o acaid
	 */
	
	// FUNÇÕES DA TELA
		
		/**
		 * Altera a justificativa, o valor da quantidade e do custo
		 * total de uma ação.
		 * 
		 * @param integer $acaid
		 * @param string $justificativa
		 * @param integer $quantidade
		 * @param integer $custo_total
		 * @return boolean
		 */
		function atualizar_dados_acao( $acaid, $justificativa, $quantidade, $custo_total )
		{
			global $db;
			$acaid = (integer) $acaid;
			$quantidade = str_replace( array( '.', ',' ), '', $quantidade );
			$custo_total = str_replace( array( '.', ',' ), '', $custo_total );
			$justificativa = str_replace( "'", "\\'", $justificativa );
			$sql = "update elabrev.ppaacao_orcamento set justificativa = '" . $justificativa . "', acaqtdefisico = " . $quantidade . ", acaqtdefinanceiro = " . $custo_total . " where acaid = " . $acaid;
			return (boolean) $db->executar( $sql );
		}
		
		/**
		 * Captura o identificador da ação que contenha os dados indicados pelo parâmetros.
		 *
		 * @param string $unicod
		 * @param string $prgcod
		 * @param string $acacod
		 * @param string $loccod
		 * @return integer
		 */
		function pegar_acaid( $unicod, $prgcod, $acacod, $loccod )
		{
			global $db;
			$sql = "select acaid from elabrev.ppaacao_orcamento where unicod = '" . $unicod . "' and prgcod = '" . $prgcod . "' and acacod = '" . $acacod . "' and loccod = '" . $loccod . "' ";
			return (integer) $db->pegaUm( $sql );
		}
		
		/**
		 * Captura unicod, unidsc, prgcod, prgdsc, acacod, loccod e
		 * locdsc de uma ação a partir de um acaid. Caso o usuário
		 * não possua permissão para mexer na Ação a função retorna
		 * false.
		 *
		 * @param integer $acaid
		 */
		function pegar_dados_acao( $acaid )
		{
			global $db;
			$acaid = (integer) $acaid;
			$sql = "select ppaacao_orcamento.unmcod, ppaacao_orcamento.procod, ppaacao_orcamento.justificativa, ppaacao_orcamento.acaqtdefisico, ppaacao_orcamento.acaqtdefinanceiro, ppaacao_orcamento.unicod, ppaacao_orcamento.unitpocod, ppaacao_orcamento.prgcod, ppaacao_orcamento.acacod, ppaacao_orcamento.acadsc, ppaacao_orcamento.loccod, unidade.unidsc, ppaprograma_orcamento.prgdsc, ppaacao_orcamento.sacdsc as locdsc from elabrev.ppaacao_orcamento inner join unidade using( unicod ) inner join elabrev.ppaprograma_orcamento using( prgcod ) where acaid = " . $acaid;
			$dados = $db->recuperar( $sql );
			if ( !$dados )
			{
				return array();
			}
			// verifica se usuário tem permissão para mexer na Ação
			$unidades_validas = $db->usuarioUnidadesPermitidas();
			if ( !in_array( $dados['unicod'], $unidades_validas ) )
			{
				return false;
			}
			return $dados;
		}
		
		/**
		 * Captura as despesas (tabela elabrev.despesaacao ) a
		 * partir de um identificador de um ação.
		 *
		 * @param integer $acaid
		 * @return string[]
		 */
		function pegar_despesaacao( $acaid )
		{
			global $db;
			$acaid = (integer) $acaid;
			$sql = "select ndpcod, iducod, foncod, idocod, dpavalor from elabrev.despesaacao join idoc using ( idoid ) join naturezadespesa using ( ndpid ) where acaid = " . $acaid;
			$dados = $db->carregar( $sql );
			return $dados ? $dados : array();
		}
		
		/**
		 * Captura o idoid a partir de um idococ da tabela idoc.
		 *
		 * @param string $idocod
		 * @return integer
		 */
		function pegar_idoid( $idocod )
		{
			global $db;
			$sql = "select idoid from idoc where idocod = '" . $idocod . "'";
			return (integer) $db->pegaUm( $sql );
		}
		
		/**
		 * Captura o npdid a partir de um npdcod da tabela naturezadespesa.
		 *
		 * @param string $ndpcod
		 * @return integer
		 */
		function pegar_npdid( $ndpcod )
		{
			global $db;
			$sql = "select ndpid from naturezadespesa where ndpcod = '" . $ndpcod . "'";
			return (integer) $db->pegaUm( $sql );
		}
		
		/**
		 * Captura a descrição do produto.
		 *
		 * @param integer $acaid
		 * @return string
		 */
		function pegar_produto( $acaid )
		{
			global $db;
			$acaid = (integer) $acaid;
			//$sql = "select prodsc from produto where procod = '" . $procod . "'";
			$sql = "
				select acadscprosof
				from elabrev.ppaacao_orcamento
				where acaid = '" . $acaid . "'
			";
			return (string) $db->pegaUm( $sql );
		}
		
		/**
		 * Captura a descrição da unidade de medida.
		 *
		 * @param integer $acaid
		 * @return string
		 */
		function pegar_unidademedida( $acaid )
		{
			global $db;
			$acaid = (integer) $acaid;
			//$sql = "select unmdsc from elabrev.unidademedida_orcamento where unmcod = '" . $unmcod . "'";
			$sql = "
				select acadscunmsof
				from elabrev.ppaacao_orcamento
				where acaid = '" . $acaid . "'
			";
			return (string) $db->pegaUm( $sql );
		}
		
		/**
		 * Remove todos os itens de despesa de uma ação.
		 *
		 * @param integer $acaid
		 * @param integer $foncod
		 * @return void
		 */
		function remover_despesaacao( $acaid, $foncod = null )
		{
			global $db;
			$acaid = (integer) $acaid;
			$sql = "delete from elabrev.despesaacao where acaid = " . $acaid;
			if ( $foncod !== null )
			{
				$sql .= " and foncod = '" . $foncod . "' ";
			}
			$db->executar( $sql );
		}
		
		
		/**
		 * @param integer $acaid
		 */
		function retorna_tipo_detalhamento( $acaid, $gnd )
		{
			global $db;
			static $tpdids = null;
			static $existe5 = false;
			static $tpdid5 = null; // indica o tpdid do momento 5
			if ( $tpdids === null )
			{
				$acaid = (integer) $acaid;
				$gnd = (integer) $gnd;
				
				$sql = "select tpdid, tpdcod from elabrev.tipodetalhamentoacao where acaid = " . $acaid . "";
				$dados = $db->carregar( $sql );
				
				$tpdids = Array();
				if ($dados)
				{
					foreach ( $dados as $dados_tpdetalhamento )
					{
						$tpdids[] = $dados_tpdetalhamento['tpdid'];
						if ( $dados_tpdetalhamento['tpdcod'] == '5' )
						{
							$tpdid5 = $dados_tpdetalhamento['tpdid'];
							$existe5 = true;
						}
					}
				}
				else
				{
					return(null);
				}
			}
			if ( count( $tpdids ) == 1 || !$existe5 )
			{
				return( $tpdids[0] );
			}
			if ( $gnd == 1 ) 
			{
				return( $tpdid5 );
			}
			else
			{
				foreach ( $tpdids as $tpdid )
				{
					if ( $tpdid != $tpdid5 )
					{
						return $tpdid;						
					}
				}	
			}
		}

		/**
		 * Insere um item na tabela elabrev.despesaacao.
		 *
		 * @param integer $acaid
		 * @param string $ndpcod
		 * @param string $iducod
		 * @param string $foncod
		 * @param integer $idocod
		 * @param float $valor
		 * @return boolean
		 */
		function inserir_despesaacao( $acaid, $ndpcod, $iducod, $foncod, $idocod, $valor )
		{
			global $db;
			static $idoids = array();
			static $npdids = array();
			$acaid = (integer) $acaid;
			$idoid = (integer) $idoid;
			$valor = (float) $valor;
			if ( !isset( $idoids[$idocod] ) )
			{
				$idoids[$idocod] = pegar_idoid( $idocod );
			}
			if ( !isset( $npdids[$ndpcod] ) )
			{
				$npdids[$ndpcod] = pegar_npdid( $ndpcod );
			}
			
			/*Executa a consulta para retornar o gnd da natureza da despesa*/
			$gnd = $ndpcod{1};
			
			/*Chama a função para retornar o tipo detalhamento que será incluído na despesa para a ação*/
			$tpdid = retorna_tipo_detalhamento($acaid, $gnd);
			if ( $tpdid === null ) $tpdid = 'null'; else $tpdid = "'" . $tpdid . "'";
			$sql = "insert into elabrev.despesaacao ( acaid, ndpid, iducod, foncod, idoid, dpavalor, tpdid ) values ( " . $acaid . ", " . $npdids[$ndpcod] . ", '" . $iducod . "', '" . $foncod . "', " . $idoids[$idocod] . ", " . $valor . " , " . $tpdid . ") ";
			return (boolean) $db->executar( $sql );
		}
		
		/**
		 * Verifica se usuário possui permissão para manipular a despesa.
		 *
		 * @param integer $acaid
		 * @param string $unicod
		 * @return booelan
		 */
		function verifica_permissao_momento( $acaid, $unicod )
		{
			global $db;
			// captura o momento
			$sql =
				"select m.mtadsc " .
				" from elabrev.ppaacao_orcamento a " .
					" inner join elabrev.momentoacao m using ( mtaid ) " .
				" where " .
					" a.acaid = ".$acaid."";
			$mtadsc = trim( $db->pegaUm( $sql ) );
			// captura o momento
			switch ( strtolower( $mtadsc ))
			{
				case 'spo': // SPO
					$cpf = $_SESSION['usucpf'];
					$sql =
						" select count(*) " .
						" from seguranca.perfilusuario " .
							" inner join seguranca.perfil using ( pflcod ) " .
						" where " .
							" usucpf = '{$cpf}' and " .
							" sisid = '2' and " .
							" pflcod not in ( " .
								" select pflcod from elabrev.tprperfil " .
							" )";
					return (boolean) $db->pegaUm( $sql );
				case 'sof': // SOF
					return false;
				default:
				case 'uo': // UO
					return $db->usuarioPossuiPermissaoUnidade( $unicod );
			}
		}
		
	// FIM FUNÇÕES DA TELA
	
	// VARIÁVEIS
		// VARIÁVEIS DA AÇÃO
			$unicod = '';
			$unicod_descricao = '';
			$prgcod = '';
			$prgcod_descricao = '';
			$acacod = '';
			$acacod_descricao = '';
			$loccod = '';
			$loccod_descricao = '';
		// FIM VARIÁVEIS DA AÇÃO
		// VARIÁVEIS FÍSICO
			$fisico_produto = '';
			$fisico_unidade_medida = '';
			$fisico_quantidade = 0;
			$fisico_total = 0;
		// FIM VARIÁVEIS FÍSICO
		// VARIÁVEIS DO FINANCEIRO
			$itens_financeiro = array();
		// FIM VARIÁVEIS DO FINANCEIRO
		// VARIÁVEIS DA JUSTIFICATIVA
			$justificativa = '';
		// FIM VARIÁVEIS DA JUSTIFICATIVA
		$unitpocod = '';
	// FIM VARIÁVEIS
	
	// MANIPULA REQUISIÇÃO
		
		// define identificador da ação que está sendo manipulada
		if ( (boolean) $_REQUEST['acaid'] )
		{
			$acaid = (integer) $_REQUEST['acaid'];
		}
		else
		{
			$acaid = pegar_acaid( $_REQUEST['unicod'], $_REQUEST['prgcod'], $_REQUEST['acacod'], $_REQUEST['loccod'] );
		}
		// FIM define identificador da ação que está sendo manipulada
		
		// alimenta variáveis
		if ( $acaid )
		{
			// garante a consistência dos dados
			// captura dados da base de dados
			$dados_acao = pegar_dados_acao( $acaid );
			// verifica se usuário tem permissão para manipular a Ação
			if  ( $dados_acao === false )
			{
				$db->insucesso( 'Você não possui permissão para detalhar esta Ação.' );
			}
			if ( count( $dados_acao ) )
			{
				$unicod           = $dados_acao['unicod'];
				$unicod_descricao = $dados_acao['unicod'] . ' - ' . $dados_acao['unidsc'];
				
				$prgcod           = $dados_acao['prgcod'];
				$prgcod_descricao = $dados_acao['prgcod'] . ' - ' . $dados_acao['prgdsc'];
				
				$acacod           = $dados_acao['acacod'];
				$acacod_descricao = $dados_acao['acacod'] . ' - ' . $dados_acao['acadsc'];
				
				$loccod           = $dados_acao['loccod'];
				$loccod_descricao = $dados_acao['loccod'] . ' - ' . $dados_acao['locdsc'];
				
				$unitpocod        = $dados_acao['unitpocod'];
				
				$fisico_quantidade     = $dados_acao['acaqtdefisico'];
				$fisico_total          = $dados_acao['acaqtdefinanceiro'];
				//$fisico_produto        = $dados_acao['procod'] . ' - ' . pegar_produto( $dados_acao['procod'] );
				$fisico_produto        = pegar_produto( $acaid );
				$fisico_unidade_medida = pegar_unidademedida( $acaid );
			}
			else
			{
				$acaid = null;
			}
		}
		// FIM alimenta variáveis
		
		// verifica, caso tenha escolhido a ação, se o usuário tem permissão pra manipular a unidade (indicada na ação)
		if ( $acaid && !$db->usuarioPossuiPermissaoUnidade( $unicod ) )
		{
			$db->insucesso( 'Você não possui permissão para manipular a unidade "' . $unicod_descricao . '".' );
		}
		// FIM verifica, caso tenha escolhido a ação, se o usuário tem permissão pra manipular a unidade (indicada na ação)
		
		// verifica se usuário pode manipular os dados
		$permissao_momento = verifica_permissao_momento( $acaid, $unicod );
		// FIM verifica se usuário pode manipular os dados
		
		
		
		/*
			define a variável $act, que define qual o status atual do ação
			null - acessando a página pela primeira vez
			'finalizar_escolha_acao' - acessou página em branco e acabou de escolher a ação
			'gravar' - ação escolhida, dados carregados... agora é só gravar na base
		*/
		$act = !isset( $_REQUEST['act'] ) ? null : $_REQUEST['act'] ;
		
		/*
			os dados das abas só devem ser carregados da base de dados nos seguintes casos:
			- usuário acabou de escolher a ação
			- usuário acabou de acessar a página e já está com a ação definida
		*/
		// carrega dados das abas da base de dados
		if ( $act == 'finalizar_escolha_acao' || ( $act == null && $acaid ) )
		{
			// captura despesas da base de dados
			foreach ( pegar_despesaacao( $acaid ) as $dados_despesa )
			{
				$novo_item_financeiro = array(
					'natureza' => $dados_despesa['ndpcod'],
					'uso'      => $dados_despesa['iducod'],
					'fonte'    => $dados_despesa['foncod'],
					'oc'       => $dados_despesa['idocod'],
					'valor'    => formata_numero( $dados_despesa['dpavalor'] )
				);
				array_push( $itens_financeiro, $novo_item_financeiro );
			}
			$dados_acao = pegar_dados_acao( $acaid );
			$justificativa = $dados_acao['justificativa'];
			$fisico_quantidade = $dados_acao['acaqtdefisico'];
			$fisico_total = $dados_acao['acaqtdefinanceiro'];
			// FIM captura despesas da base de dados
		}
		// carrega dados das abas do formulário e tenta gravar
		else if ( $act == 'gravar' && $permissao_momento )
		{
			$financeiro_total = 0; // inicia variável utilizada para ser verificada com o valor total do físico
			$financeiro_total_por_fonte = array();
			// alimenta variáveis do financeiro com os dados do formulário submetido
			if ( isset( $_REQUEST['natureza'] ) && count( $_REQUEST ) )
			{
				foreach ( $_REQUEST['natureza'] as $indice_item_financeiro => $valor_natureza )
				{
					$valor_fonte = $_REQUEST['fonte'][$indice_item_financeiro];
					$novo_item_financeiro = array(
						'natureza' => $valor_natureza,
						'uso'      => $_REQUEST['uso'][$indice_item_financeiro],
						'fonte'    => $valor_fonte,
						'oc'       => $_REQUEST['oc'][$indice_item_financeiro],
						'valor'    => $_REQUEST['valor'][$indice_item_financeiro]
					);
					array_push( $itens_financeiro, $novo_item_financeiro );
					$valor_valor = str_replace( '.', '', $_REQUEST['valor'][$indice_item_financeiro] );
					$financeiro_total += $valor_valor;
					if ( !array_key_exists( $valor_fonte, $financeiro_total_por_fonte ) )
					{
						$financeiro_total_por_fonte[$valor_fonte] = 0;
					}
					$financeiro_total_por_fonte[$valor_fonte] += $valor_valor;
				}
			}
			$verificacao_saldo = true;
			
			foreach ( $financeiro_total_por_fonte as $financeiro_foncod => $financeiro_valor_por_fonte )
			{
				// verifica se a fonte está disponível (possui limite)
				$sql_verifica_saldo =
					" select " .
						" saldo, vllimite " .
					" from elabrev.v_saldounidadefonte2 " .
					" where " .
						" unicod = '" . $unicod . "' and " .
						" ppoanoexercicio = '" . $_SESSION['exercicio'] . "' and " .
						" foncod = '" . $financeiro_foncod . "' ";
				$saldounidadefonte = $db->recuperar( $sql_verifica_saldo );
				if ( !$saldounidadefonte )
				{
					$verificacao_saldo = false;
					?>
						<script type="text/javascript" language="javascript">
							alert( 'Atenção, a fonte <?= $financeiro_foncod ?> não possui limite para esta Unidade.' );
						</script>
					<?
					break;
				}
				// FIM verifica se a fonte está disponível (possui limite)
				
				// verifica se saldo ultrapassa limite
				$saldounidadefonte['saldo'] = (integer) $saldounidadefonte['saldo'];
				$saldo_por_fonte = $saldounidadefonte['saldo'];
				$limite_por_fonte = $saldounidadefonte['vllimite'];
				$sql_distribuido_fonte_acao = "select sum( dpavalor ) as valor from elabrev.despesaacao where foncod = '" . $financeiro_foncod . "' and acaid = " . $acaid;
				$distribuido_fonte_acao = $db->pegaUm( $sql_distribuido_fonte_acao );
				$saldo_por_fonte += $distribuido_fonte_acao;
				$saldo_por_fonte -= $financeiro_valor_por_fonte;
				
				// verifica se saldo negativo
				if ( $saldo_por_fonte < 0 )
				{
					// exibe erro quando saldo (indicado pelo usuário) é menor que o saldo anterior (base de dados)
					if ( $saldo_por_fonte < $saldounidadefonte['saldo'] )
					{
						$verificacao_saldo = false;
						?>
							<script type="text/javascript" language="javascript">
								alert( 'Atenção, o valor distribuído na fonte <?= $financeiro_foncod ?> ultrapassa o saldo da unidade.\n\nLimite: R$ <?= formata_valor( $limite_por_fonte, 2 ) ?>\n\nDistribuído: R$ <?= formata_valor( $financeiro_valor_por_fonte, 2 ) ?>\n\nDiferença / Saldo na Fonte <?= $financeiro_foncod ?>: R$ <?= formata_valor( $saldo_por_fonte, 2 ) ?>\n' );
							</script>
						<?
						break;
					}
					// FIM exibe erro quando saldo (indicado pelo usuário) é menor que o saldo anterior (base de dados)
				}
				// FIM verifica se saldo negativo
				
				// FIM verifica se saldo ultrapassa limite
			}
			
			// alimenta variáveis da justifivativa e físico com os dados do formulário submetido
			$justificativa = $_REQUEST['justificativa'];
			$fisico_quantidade = $_REQUEST['fisico_quantidade'];
			$fisico_total = $_REQUEST['fisico_total'];
			// FIM alimenta variáveis da justifivativa e físico com os dados do formulário submetido
			// FIM alimenta variáveis do financeiro com os dados do formulário submetido
			if ( $verificacao_saldo == true )
			{
				// realiza requisições à base de dados
				$sucesso = false;
				if ( $acaid && $financeiro_total <= str_replace( '.', '', $fisico_total ) )
				{
					$sucesso = true;
					remover_despesaacao( $acaid );
					foreach ( $itens_financeiro as $item_financeiro )
					{
						$valor = str_replace( '.', '', $item_financeiro['valor'] );
						if ( !inserir_despesaacao( $acaid, $item_financeiro['natureza'], $item_financeiro['uso'], $item_financeiro['fonte'], $item_financeiro['oc'], $valor ) )
						{
							$sucesso = false;
							break;
						}
					}
					if ( $sucesso == true )
					{
						$sucesso = atualizar_dados_acao( $acaid, $justificativa, $fisico_quantidade, $fisico_total );
					}
				}
				if ( $sucesso == true )
				{
					$db->commit();
					header( "Location: elabrev.php?modulo=principal/propostaorcamentaria/despesadetalhamento&acao=A&acaid=" . $acaid );
					exit();
				}
				// FIM realiza requisições à base de dados
			}
		}
		// FIM verifica a forma como o formulário foi submetido
	// FIM MANIPULA REQUISIÇÃO
	

	
	
	//Verifica se possui acesso a todas as unidades, caso sim pode visualizar todas as unidades, 
	//												 caso contrário visualiza as naturezas exceto o gndcod <> 1
	$habilita_gnd_1 = $db->usuarioPossuiPermissaoTodasUnidades();
	$where_natureza = $habilita_gnd_1 ? '' : ' and gndcod <> 1 and gndcod in ( select gndcod from elabrev.v_unidadegnd where unicod = ' . $unicod . ' ) ';
	
	// QUERYS PARA OS CAMPOS AJAX
		$sql_acao_unidade = "select distinct unidade.unicod as valor, unidade.unidsc as descricao from elabrev.ppaacao_orcamento join elabrev.usuarioresponsabilidade using ( unicod ) join unidade using ( unicod ) where unidade.unicod like '%s%' and unidade.unistatus = 'A' and unidade.unitpocod = 'U' and ppaacao_orcamento.acastatus = 'A' and acasnrap = false order by unidade.unicod";
		$sql_acao_programa = "select distinct ppaprograma_orcamento.prgcod as valor, ppaprograma_orcamento.prgdsc as descricao from elabrev.ppaacao_orcamento inner join unidade on ( ppaacao_orcamento.unicod= unidade.unicod ) inner join elabrev.ppaprograma_orcamento on ( ppaacao_orcamento.prgcod= ppaprograma_orcamento.prgcod ) where unidade.unicod like '%s%%' and unidade.unistatus = 'A' and unidade.unitpocod = 'U' and ppaprograma_orcamento.prgcod like '%s%%' and ppaprograma_orcamento.prgstatus = 'A' and ppaacao_orcamento.acastatus = 'A' and acasnrap = false order by ppaprograma_orcamento.prgcod";
		$sql_acao_acao = "select distinct ppaacao_orcamento.acacod as valor, ppaacao_orcamento.acadsc as descricao from elabrev.ppaacao_orcamento inner join unidade on ( ppaacao_orcamento.unicod= unidade.unicod ) inner join elabrev.ppaprograma_orcamento on ( ppaacao_orcamento.prgcod= ppaprograma_orcamento.prgcod ) where unidade.unicod like '%s%%' and unidade.unistatus = 'A' and unidade.unitpocod = 'U' and ppaprograma_orcamento.prgcod like '%s%%' and ppaprograma_orcamento.prgstatus = 'A' and upper( ppaacao_orcamento.acacod ) like upper( '%s%%' ) and ppaacao_orcamento.acastatus = 'A' and acasnrap = false order by ppaacao_orcamento.acacod";
		$sql_acao_localizador = "select distinct ppaacao_orcamento.loccod as valor, ppaacao_orcamento.sacdsc as descricao from elabrev.ppaacao_orcamento inner join unidade on ( ppaacao_orcamento.unicod= unidade.unicod ) inner join elabrev.ppaprograma_orcamento on ( ppaacao_orcamento.prgcod= ppaprograma_orcamento.prgcod ) where unidade.unicod like '%s%%' and unidade.unistatus = 'A' and unidade.unitpocod = 'U' and ppaprograma_orcamento.prgcod like '%s%%' and ppaprograma_orcamento.prgstatus = 'A' and upper( ppaacao_orcamento.acacod ) like upper( '%s%%' ) and ppaacao_orcamento.loccod like '%s%%' and ppaacao_orcamento.acastatus = 'A' and acasnrap = false order by ppaacao_orcamento.loccod";
		//$sql_financeiro_natureza = "select ndpcod as valor, ndpcod || ' - ' || ndpdsc as descricao from naturezadespesa where ndpcod like '%s%' and edpcod != '00' and ndpstatus = 'A' $where order by ndpcod";
		$sql_financeiro_natureza = "select ndpcod as codigo, ndpdsc as descricao from naturezadespesa where edpcod != '00' and ndpstatus = 'A' " . $where_natureza . " order by ndpcod";
		//$sql_financeiro_uso = "select iducod as valor, iducod || ' - ' || idudsc as descricao from identifuso where iducod like '%s%' and idustatus = 'A' order by iducod";
		$sql_financeiro_uso = "select iducod as codigo, idudsc as descricao from identifuso where idustatus = 'A' order by iducod";
		//$sql_financeiro_fonte = "select foncod as valor, foncod || ' - ' || fondsc as descricao from fonterecurso where foncod like '%s%' and fonstatus = 'A' and foncod in ( select foncod from elabrev.limitefonte where limitefonte.unicod = '" . $unicod . "' and limitefonte.unitpocod = '" . $unitpocod . "' ) order by foncod";
		$sql_financeiro_fonte = "select foncod as codigo, fondsc as descricao from public.fonterecurso where fonstatus = 'A' and foncod in ( select foncod from elabrev.limitefonte where limitefonte.unicod = '" . $unicod . "' and limitefonte.unitpocod = '" . $unitpocod . "' ) order by foncod";
		//$sql_financeiro_oc = "select idocod as valor, idocod || ' - ' || idodsc as descricao from idoc where idocod like '%s%' and idostatus = 'A' order by idodsc";
		$sql_financeiro_oc = "select idocod as codigo, idodsc as descricao from idoc where idostatus = 'A' order by idodsc";
	// FIM QUERYS PARA OS CAMPOS AJAX
	
	include APPRAIZ . "includes/cabecalho.inc";
	
?>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<script type="text/javascript" src="/includes/abas.js"></script>
<script type="text/javascript" language="javascript">

	/**
	 * Indica se o usuário alterou em algum momento os valores de
	 * distribuição. Caso o usuário altere o valore original e
	 * posteriormente retorne ao valor original esta variável será
	 * verdadeira.
	 * 
	 * @var boolean
	 */
	var houve_modificacao = false;

	/**
	 * Redireciona para a tela de seleção de ações.
	 * 
	 * @return void
	 */
	function selecionar_outra_acao()
	{
		//document.location.href = 'elabrev.php?modulo=principal/propostaorcamentaria/despesa&acao=A';
		document.location.href = 'elabrev.php?modulo=principal/propostaorcamentaria/despesaremanejamento&acao=A';
	}
	
</script>
<br/>
<?$db->cria_aba($abacod_tela,$url,'');?>
<? monta_titulo( $titulo_modulo, '' ); ?>
<?
//Define a unidade atual na seção para troca de abas
$_SESSION["unicod_visualizacao"]=$unicod;?>
<form name="formulario" method="post" style="margin:0px;">
	<input type="hidden" name="act" value="" />
	<input type="hidden" name="acaid" value="<?= $acaid ?>" />
	<input type="hidden" name="aba_selecionada" value="<?= $aba_selecionada ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="20%" align="right" class="SubTituloDireita" >Unidade:</td>
			<td width="80%">
				<? if ( $acaid ) : ?>
					<?= $unicod_descricao ?>
					<input type="hidden" name="unicod" id="unicod" value="<?= $unicod ?>" />
				<? else : ?>
					<?= campo_texto_ajax_cascata( $sql_acao_unidade, 'unicod', 'Unidade', array(), 10, 5, '', 'left', false, $unicod, $unicod_descricao ) ?>
				<? endif; ?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" >Programa:</td>
			<td>
				<? if ( $acaid ) : ?>
					<?= $prgcod_descricao ?>
					<input type="hidden" name="prgcod" id="prgcod" value="<?= $prgcod ?>" />
				<? else : ?>
					<?= campo_texto_ajax_cascata( $sql_acao_programa, 'prgcod', 'Programa', array( 'unicod' ), 10, 4, '', 'left', false, $prgcod, $prgcod_descricao ) ?>
				<? endif; ?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" >Ação:</td>
			<td>
				<? if ( $acaid ) : ?>
					<?= $acacod_descricao ?>
					<input type="hidden" name="acacod" id="acacod" value="<?= $acacod ?>" />
				<? else : ?>
					<?= campo_texto_ajax_cascata( $sql_acao_acao, 'acacod', 'Ação', array( 'unicod', 'prgcod' ), 10, 4, '', 'left', false, $acacod, $acacod_descricao ) ?>
				<? endif; ?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" >Localizador:</td>
			<td>
				<? if ( $acaid ) : ?>
					<?= $loccod_descricao ?>
					<input type="hidden" name="loccod" id="loccod" value="<?= $loccod ?>" />
				<? else : ?>
					<?= campo_texto_ajax_cascata( $sql_acao_localizador, 'loccod', 'Localizador', array( 'unicod', 'prgcod', 'acacod' ), 10, 4, '', 'left', false, $loccod, $loccod_descricao ) ?>
				<? endif; ?>
			</td>
		</tr>
		<? if ( $acaid ) : ?>
			<tr>
				<td align="right" class="SubTituloDireita" >Total da despesa</td>
				<td>
					<span id="valor_total_despesa"></span>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" >&nbsp;</td>
				<td>
					<input type="button" name="botao_selecionar_outra_acao" value="Selecionar outra Ação" onclick="selecionar_outra_acao();" />
				</td>
			</tr>
		<? endif; ?>
	</table>
	<script type="text/javascript">
		
		// JAVASCRIPT AÇÃO
			
			/**
			 * Finaliza a escolha da ação. Todos os campos utilizados
			 * para a escolha da ação são desabilitados. Caso a ação não
			 * tenha sido escolhida é retornado true e nenhum bloqueio é
			 * efetuado.
			 * 
			 * @return boolean
			 */
			function bloquear_escolha_acao()
			{
				var campo_prgcod = document.getElementById( 'prgcod' );
				var campo_acacod = document.getElementById( 'acacod' );
				var campo_loccod = document.getElementById( 'loccod' );
				var unicod = document.getElementById( 'unicod' ).value;
				var prgcod = campo_prgcod.value;
				var acacod = campo_acacod.value;
				var loccod = campo_loccod.value;
				if ( !unicod || !prgcod || !acacod || !loccod )
				{
					alert( 'Escolha a ação.' );
					return false;
				}
				bloquear_escolha_unidade();
				var lsImagem_prgcod = document.getElementById( 'lsImagem_prgcod' );
				if ( lsImagem_prgcod )
				{
					document.getElementById( 'lsImagem_prgcod' ).onclick = null;
				}
				campo_prgcod.onfocus = new Function( "this.blur();" );
				var lsImagem_acacod = document.getElementById( 'lsImagem_acacod' );
				if ( lsImagem_acacod )
				{
					document.getElementById( 'lsImagem_acacod' ).onclick = null;
				}
				campo_acacod.onfocus = new Function( "this.blur();" );
				var lsImagem_loccod = document.getElementById( 'lsImagem_loccod' );
				if ( lsImagem_loccod )
				{
					document.getElementById( 'lsImagem_loccod' ).onclick = null;
				}
				campo_loccod.onfocus = new Function( "this.blur();" );
				document.getElementById( 'botao_finalizar_acao' ).style.display = 'none';
				// altera status do formulário
				document.formulario.act.value = 'finalizar_escolha_acao';
				return true;
			}
			
			/**
			 * Finaliza a escolha da unidade. O campo para a escolha da
			 * unidade da ação é desabilitado.
			 * 
			 * @return boolean
			 */
			function bloquear_escolha_unidade()
			{
				var lsImagem_unicod = document.getElementById( 'lsImagem_unicod' );
				if ( lsImagem_unicod )
				{
					document.getElementById( 'lsImagem_unicod' ).onclick = null;
				}
				var unicod = document.getElementById( 'unicod' );
				if ( unicod )
				{
					document.getElementById( 'unicod' ).onfocus = new Function( "this.blur();" );
				}
			}
			
			/**
			 * Fianliza a escolha de ação e submete o formulário.
			 * 
			 * @return void
			 */
			function submeter_escolha_acao()
			{
				if ( bloquear_escolha_acao() == true )
				{
					submeter_formulario();
				}
			}
			
		// FIM JAVASCRIPT AÇÃO
		
	</script>
	<p id="botao_finalizar_acao" style="text-align: center; <?= $acaid ? 'display: none;' : '' ?>">
		<input type="button" name="finalizar_acao" value="Finalizar Escolha da Ação" onclick="javascript:submeter_escolha_acao();" />
	</p>
	<div id="abas" style="display: <?= $acaid ? 'block' : 'none' ?>;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Cancelamento</li>
			</ul>
		</div>
		<!-- FINANCEIRO -->
		<div id="aba_financeiro" class="conteudoAba">
			<table width="80%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td width="100" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Natureza</strong></td>
						<td width="75" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IDUSO</strong></td>
						<td width="75" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Fonte</strong></td>
						<td width="75" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IDOC</strong></td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor (R$ 1,00)</strong></td>
						<td width="81" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					</tr>
				</thead>
				<tr>
					<td colspan="6">					
						<div class="div_rolagem" id="div_tabela_financeiro" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
							<table id="tabela_financeiro_cancelamento" width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
								<tr valign="bottom">
									<td width="100" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="78" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="78" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="78" style="height: 0; margin: 0; padding: 0;"></td>
									<td style="height: 0; margin: 0; padding: 0;"></td>
									<td width="32" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="32" style="height: 0; margin: 0; padding: 0;"></td>
								</tr>
							</table>
						</div>
					</td>
				</tr>
				<tr valign="bottom">
					<td align="right" colspan="4" style="color: #ababab; border-top: 3px solid #dfdfdf;">Total</td>
					<td align="right" style="color: #0066cc; border-top: 3px solid #dfdfdf;" valign="bottom">
						<?= campo_texto( 'item_financeiro_valor_total_cancelamento', 'N', 'N', 'Valor Total', 25, 20, '', '', 'right', '', 0, ' id="item_financeiro_valor_total_cancelamento" ' ) ?>
					</td>
					<td style="border-top: 3px solid #dfdfdf;">&nbsp;</td>
				</tr>
			</table>
			<? if ( $permissao_momento ) : ?>
				<br/><br/>
				<input type="button" name="salvar" value="Salvar" onclick="javascript:validar_cadastro();"/>
			<? endif; ?>
		</div>
	</div>
	
	
	
	<div id="abas" style="display: <?= $acaid ? 'block' : 'none' ?>;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Suplementação</li>
			</ul>
		</div>
		<!-- FINANCEIRO -->
		<div id="aba_financeiro" class="conteudoAba">
			<table width="80%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td width="100" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Natureza</strong></td>
						<td width="75" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IDUSO</strong></td>
						<td width="75" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Fonte</strong></td>
						<td width="75" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IDOC</strong></td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor (R$ 1,00)</strong></td>
						<td width="81" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					</tr>
				</thead>
				<? if ( $permissao_momento ) : ?>
					<tr bgcolor="#f7f7f7">
						<td width="100">
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'natureza\' );" '; ?>
							<? texto_popup( 'nova_natureza', $sql_financeiro_natureza, 'Natureza da Despesa', 8, 13, '########', '', $complemento ); ?>
						</td>
						<td width="75">
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'iduso\' );" '; ?>
							<? texto_popup( 'novo_uso', $sql_financeiro_uso, 'Ident. de Uso', 1, 7, '#', '', $complemento ); ?>
						</td>
						<td width="75">
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'fonte\' );" '; ?>
							<? texto_popup( 'nova_fonte', $sql_financeiro_fonte, 'Fonte de Recursos', 3, 7, '###', '', $complemento ); ?>
						</td>
						<td width="75">
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'idoc\' );" '; ?>
							<? texto_popup( 'novo_oc', $sql_financeiro_oc, 'Ident. Op. de Crédito', 4, 7, '####', '', $complemento ); ?>
						</td>
						<td align="right">
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'valor\' );" '; ?>
							<?= campo_texto( 'novo_valor', 'N', 'S', '', 25, 14, '##.###.###.###', '', 'right', '', 0, ' id="novo_valor" onblur="MouseBlur(this);" ' . $complemento ); ?>
						</td>
						<td width="81" align="left">
							<a href="javascript:item_financeiro_adicionar_do_formulario();" title="Incluir" style="margin: 0; padding: 0 6px;">
								<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
							</a>
						</td>
					</tr>
				<? else : ?>
					<input type="hidden" value="" name="nova_natureza" id="nova_natureza"/>
					<input type="hidden" value="" name="novo_uso" id="novo_uso"/>
					<input type="hidden" value="" name="nova_fonte" id="nova_fonte"/>
					<input type="hidden" value="" name="novo_oc" id="novo_oc"/>
					<input type="hidden" value="" name="novo_valor" id="novo_valor"/>
				<? endif; ?>
				<tr>
					<td colspan="6">					
						<div class="div_rolagem" id="div_tabela_financeiro" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
							<table id="tabela_financeiro_suplementacao" width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
								<tr valign="bottom">
									<td width="100" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="78" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="78" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="78" style="height: 0; margin: 0; padding: 0;"></td>
									<td style="height: 0; margin: 0; padding: 0;"></td>
									<td width="32" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="32" style="height: 0; margin: 0; padding: 0;"></td>
								</tr>
							</table>
						</div>
					</td>
				</tr>
				<tr valign="bottom">
					<td align="right" colspan="4" style="color: #ababab; border-top: 3px solid #dfdfdf;">Total</td>
					<td align="right" style="color: #0066cc; border-top: 3px solid #dfdfdf;" valign="bottom">
						<?= campo_texto( 'item_financeiro_valor_total_suplementacao', 'N', 'N', 'Valor Total', 25, 20, '', '', 'right', '', 0, ' id="item_financeiro_valor_total_suplementacao" ' ) ?>
					</td>
					<td style="border-top: 3px solid #dfdfdf;">&nbsp;</td>
				</tr>
				<tr valign="bottom">
					<td align="right" colspan="6" style="border-top: 3px solid #dfdfdf;">
						<input type="button" onclick="javascript:MostraRelatorio(<?=$unicod?>);" value="Visualizar saldos" >
					</td>
				</tr>
			</table>
			<? if ( $permissao_momento ) : ?>
				<br/><br/>
				<input type="button" name="salvar" value="Salvar" onclick="javascript:validar_cadastro();"/>
			<? endif; ?>
		</div>
	</div>
</form>
<script type="text/javascript">
	
	/**
	 * Indica de usuário possui privilégio para manipular os dados de despesa.
	 * 
	 * @var boolean
	 */
	var permissao_momento = <?= $permissao_momento ? 'true' : 'false' ?>;
	
	// JAVASCRIPT FINANCEIRO
		
		/**
		 * Contém os itens financeiros adicionados. Utilizado para
		 * evitar repetições de natureza-iduso-fonte-idoc na tabela
		 * de item financeiro.
		 * 
		 * @var boolean[]
		 */
		var item_financeiros_adicionados = new Array();
		
		/**
		 * Soma de todos os itens de cada tabela de despesas.
		 * 
		 * @var float
		 */
		var item_financeiro_valor_total = new Array();
			item_financeiro_valor_total['cancelamento'] = 0;
			item_financeiro_valor_total['suplementacao'] = 0;
		
		/**
		 * Indica se usuário pode manipular itens financeiros onde
		 * as natureza possuem gnd = 1.
		 * 
		 * @var boolean[]
		 */
		var item_financeiro_habilita_gnd_1 = <?= $habilita_gnd_1 ? 'true' : 'false' ?>;
		
		/**
		 * Sufixo do identificador de cada registro na tabela de
		 * financeiro. Variável incremental utilizada para evitar
		 * identificadores repetidos para cada tag 'tr' da tabela
		 * de financeiro.
		 * 
		 * @var float
		 */
		var item_financeiro_id_atual = new Array();
			item_financeiro_id_atual['cancelamento'] = 0;
			item_financeiro_id_atual['suplementacao'] = 0;
		
		/**
		 * Indica se existia itens financeiros ao iniciar a tela.
		 * 
		 * @var float
		 */
		var possui_item_financeiro_inicial = <?= count( $itens_financeiro ) ? 'true' : 'false' ?>;
		
		// VALORES DOS CAMPOS ITEM FINANCEIRO VÁLIDOS
		var financeiro_naturezas_validas = new Array();
		<?
			$dados_temp = $db->carregar( $sql_financeiro_natureza );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?> financeiro_naturezas_validas['<?= $dados_temp_valor['codigo'] ?>'] = true; <?
				}
			}
		?>
		var financeiro_usos_validos = new Array();
		<?
			$dados_temp = $db->carregar( $sql_financeiro_uso );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?>
						financeiro_usos_validos['<?= $dados_temp_valor['codigo'] ?>'] = true;
					<?
				}
			}
		?>
		var financeiro_fontes_validas = new Array();
		var financeiro_fontes_validas_quantidade = 0;
		<?
			$dados_temp = $db->carregar( $sql_financeiro_fonte );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?>
						financeiro_fontes_validas['<?= $dados_temp_valor['codigo'] ?>'] = true;
						financeiro_fontes_validas_quantidade++;
					<?
				}
			}
		?>
		var financeiro_ocs_validos = new Array();
		<?
			$dados_temp = $db->carregar( $sql_financeiro_oc );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?>
						financeiro_ocs_validos['<?= $dados_temp_valor['codigo'] ?>'] = true;
					<?
					
				}
			}
		?>
		
		function item_financeiro_hilight( id_tr, loop )
		{
			var tr = document.getElementById( id_tr );
			if ( !tr )
			{
				return;
			}
			if ( loop < 1 )
			{
				item_financeiro_organizar_cor_linha();
				return;
			}
			var cor = '';
			switch ( loop-- % 9 )
			{
				case 0:	case 8:
					//cor = '#d0d0d0';
					cor = '#ffffff';
					break;
				case 1:	case 7:
					//cor = '#d8d8d8';
					cor = '#ffffdd';
					break;
				case 2:	case 6:
					//cor = '#e0e0e0';
					cor = '#ffffaa';
					break;
				case 3:	case 5:
					//cor = '#e8e8e8';
					cor = '#ffff80';
					break;
				case 4:
					//cor = '#f0f0f0';
					cor = '#ffff40';
					break;
			}
			tr.style.backgroundColor = cor;
			var funcao_hilight = new Function( ' item_financeiro_hilight( \'' + id_tr + '\', ' + loop + ' ); ' );
			setTimeout( funcao_hilight, 120 );
		}
		
		/**
		 * Adiciona um item à tabela de itens de despesa. Também é adiciona
		 * um campo no formulário para as linhas.
		 * 
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @return boolean
		 */
		function item_financeiro_adicionar( destino, natureza, uso, fonte, oc, valor )
		{
			if ( valor <= 0 )
			{
				alert( 'Valor deve ser maior que zero.' );
				document.getElementById( 'novo_valor' ).focus();
				return false;
			}
			var id_novo_item = natureza + uso + fonte + oc;
			// verifica se item já existe
			if ( item_financeiros_adicionados[id_novo_item] )
			{
				alert( 'Já existe item financeiro para esta natureza, fonte, iduso e idoc. Você pode alterar o valor desse item selecionando o item correspondente na tabela abaixo do formulário.' );
				// posiciona a linha a ser destacada
				document.getElementById( item_financeiros_adicionados[id_novo_item] + '_ancora' ).focus();
				var funcao_hilight = new Function( ' item_financeiro_hilight( \'' + item_financeiros_adicionados[id_novo_item] + '\', 30 ); ' );
				setTimeout( funcao_hilight, 300 );
				return false;
				
			}
			// define dados gerais do item a ser adicionado
			item_financeiro_id_atual[destino]++;
			var tabela = document.getElementById( 'tabela_financeiro_' + destino );
			var id = 'item_financeiro_' + destino + '_' + item_financeiro_id_atual[destino];
			item_financeiros_adicionados[id_novo_item] = id;
			// FIM define dados gerais do item a ser adicionado
			// cria nova linha para a tabela de financeiro
			var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
			    nova_linha.id = id;
			    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
			// FIM cria nova linha para a tabela de financeiro
			// cria as célular da nova linha
			var celula_natureza = nova_linha.insertCell( 0 );
			var celula_uso = nova_linha.insertCell( 1 );
			var celula_fonte = nova_linha.insertCell( 2 );
			var celula_oc = nova_linha.insertCell( 3 );
			var celula_valor = nova_linha.insertCell( 4 );
			    celula_valor.style.textAlign = 'right';
			    celula_valor.style.color = '#0066cc';
			var celula_acao_alterar = nova_linha.insertCell( 5 );
			    celula_acao_alterar.style.textAlign = 'center';
			var celula_acao_remover = nova_linha.insertCell( 6 );
			    celula_acao_remover.style.textAlign = 'center';
			// FIM cria as célular da nova linha
			// cria campos ocultos com os dados do novo item
			var input_hidden_natureza = item_financeiro_criar_input( natureza, 'natureza' );
			var input_hidden_uso = item_financeiro_criar_input( uso, 'uso' );
			var input_hidden_fonte = item_financeiro_criar_input( fonte, 'fonte' );
			var input_hidden_oc = item_financeiro_criar_input( oc, 'oc' );
			var input_hidden_valor = item_financeiro_criar_input( valor, 'valor' );
				input_hidden_valor.id = id + '_valor';
			// FIM cria campos ocultos com os dados do novo item
			// define dado para as células de ação
			//if ( !permissao_momento || ( item_financeiro_habilita_gnd_1 == false && natureza.substr( 1, 1 ) == '1' ) )
			if ( destino == 'cancelamento' )
			{
				link_acao_alterar = document.createElement( 'font' );
				link_acao_alterar.innerHTML = ' - ';
			}
			else
			{
				var link_acao_alterar = document.createElement( 'a' );
				    link_acao_alterar.href = 'javascript:item_financeiro_alterar( \'' + id + '\', \'' + natureza + '\', \'' + uso + '\', \'' + fonte + '\', \'' + oc + '\', \'' + valor + '\' );';
				    link_acao_alterar.title = 'Editar';
				    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
			}
			var link_acao_remover = document.createElement( 'a' );
			    link_acao_remover.href = 'javascript:item_financeiro_remover( \'' + destino + '\', \'' + id + '\', true );';
			    link_acao_remover.title = 'Excluir';
			    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';
			link_acao_alterar.id = id + '_ancora';
			// FIM define dado para as células de ação
			// insere os dados ( textos e inputs ) nas células
			celula_natureza.appendChild( document.createTextNode( natureza ) );
			celula_natureza.appendChild( input_hidden_natureza );
			celula_uso.appendChild( document.createTextNode( uso ) );
			celula_uso.appendChild( input_hidden_uso );
			celula_fonte.appendChild( document.createTextNode( fonte ) );
			celula_fonte.appendChild( input_hidden_fonte );
			celula_oc.appendChild( document.createTextNode( oc ) );
			celula_oc.appendChild( input_hidden_oc );
			celula_valor.appendChild( document.createTextNode( valor ) );
			celula_valor.appendChild( input_hidden_valor );
			if ( link_acao_alterar != null  )
			{
				celula_acao_alterar.appendChild( link_acao_alterar );
			}
			if ( link_acao_remover != null  )
			{
				celula_acao_remover.appendChild( link_acao_remover );
			}
			// FIM insere os dados ( textos e inputs ) nas células
			// altera soma dos valores de acordo com o novo item
			item_financeiro_valor_total[destino] += desformata_decimal( valor );
			if ( destino == 'cancelamento' )
			{
				item_financeiro_valor_total['total'] = item_financeiro_valor_total[destino];
			}
			item_financeiro_atualizar_valor_total( destino );
			// FIM altera soma dos valores de acordo com o novo item
			// atualiza as cores alternadas das linhas
			item_financeiro_organizar_cor_linha( destino );
			return true;
		}
		
		/**
		 * Insere item financeiro na tabela de financeiro com
		 * os dados atuais do formulário. As verificações de
		 * preenchimento dos dados é realizada nesta função.
		 * 
		 * @return void
		 */
		function item_financeiro_adicionar_do_formulario()
		{
			// captura os campos do financeiro
			var natureza = document.getElementById( 'nova_natureza' );
			var uso = document.getElementById( 'novo_uso' );
			var fonte = document.getElementById( 'nova_fonte' );
			var oc = document.getElementById( 'novo_oc' );
			var valor = document.getElementById( 'novo_valor' );
			// realiza validação dos dados
			if ( !validaBranco( natureza, 'Natureza da Despesa' ) ) return;
			if ( !validaBranco( uso, 'Ident. de Uso' ) ) return;
			if ( !validaBranco( fonte, 'Fontes de Recursos' ) ) return;
			if ( !validaBranco( oc, 'Ident. Op. de Crédito' ) ) return;
			if ( !validaBranco( valor, 'Valor da Proposta' ) ) return;
			if ( !financeiro_naturezas_validas[natureza.value] )
			{
				alert( 'Natureza de Despesa inválida.' );
				natureza.focus();
				natureza.select();
				return;
			}
			if ( !financeiro_usos_validos[uso.value] )
			{
				alert( 'Ident. de Uso inválido.' );
				uso.focus();
				uso.select();
				return;
			}
			if ( !financeiro_fontes_validas[fonte.value] )
			{
				alert( 'Fonte inválida.' );
				fonte.focus();
				fonte.select();
				return;
			}
			if ( !financeiro_ocs_validos[oc.value] )
			{
				alert( 'Ident. Op. de Crédito inválido.' );
				oc.focus();
				oc.select();
				return;
			}
			// insere os dados na tabela de financeiro
			if ( item_financeiro_adicionar( 'suplementacao', natureza.value, uso.value, fonte.value, oc.value, valor.value ) == false )
			{
				return;
			}
			// limpa campos para nova inserção
			natureza.value = '';
			uso.value = '';
			fonte.value = '';
			oc.value = '';
			valor.value = '';
			//item_financeiro_atualizar_fisico();
			item_financeiro_preencher_valores_default();
			houve_modificacao = true;
			document.getElementById( 'nova_natureza' ).focus();
		}
		
		/**
		 * Prepara formulário para edição de um item da tabela financeiro.
		 * 
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @return void
		 */
		function item_financeiro_alterar( destino, id_tr, natureza, uso, fonte, oc, valor )
		{
			houve_modificacao = true;
			// captura os campos do financeiro
			var campo_natureza = document.getElementById( 'nova_natureza' );
			var campo_uso = document.getElementById( 'novo_uso' );
			var campo_fonte = document.getElementById( 'nova_fonte' );
			var campo_oc = document.getElementById( 'novo_oc' );
			var campo_valor = document.getElementById( 'novo_valor' );
			campo_natureza.value = natureza;
			campo_uso.value = uso;
			campo_fonte.value = fonte;
			campo_oc.value = oc;
			campo_valor.value = valor;
			item_financeiro_remover( destino, id_tr, false );
			// retira item da lista dos já inseridos
			var id_novo_item = natureza + uso + fonte + oc;
			item_financeiros_adicionados[id_novo_item] = false;
		}
		
		/**
		 * Detecta qual o campo focado atualmente e altera para o
		 * próxima caso a tecla apertada seja 'enter'.
		 * 
		 * @param void
		 * @return void
		 */
		function item_financeiro_alterar_campo_focado( event, campo )
		{
			if ( event.keyCode != 13 )
			{
				return;
			}
			switch ( campo )
			{
				case 'natureza':
					var campo = document.getElementById( 'novo_uso' );
					break;
				case 'iduso':
					var campo = document.getElementById( 'nova_fonte' );
					break;
				case 'fonte':
					var campo = document.getElementById( 'novo_oc' );
					break;
				case 'idoc':
					var campo = document.getElementById( 'novo_valor' );
					break;
				case 'valor':
					item_financeiro_adicionar_do_formulario();
					return;
				default:
					return;
			}
			campo.focus();
			campo.select();
		}
		
		/**
		 * Atualiza o dado da tela de acordo com a variável
		 * item_financeiro_valor_total.
		 * 
		 * @param string
		 * @param void
		 * @return void
		 */
		function item_financeiro_atualizar_valor_total( destino )
		{
			var valor_total_string = item_financeiro_valor_total[destino] + '';
			valor_total_string = MascaraMonetario( valor_total_string );
			valor_total_string = valor_total_string.substr( 0, valor_total_string.length - 3 );
			//document.getElementById( 'item_financeiro_valor_total' ).innerHTML = valor_total_string;
			document.getElementById( 'item_financeiro_valor_total_' + destino ).value = valor_total_string;
		}
		
		/**
		 * Organiza as cores da linhas, ora com valor vazio ora com 'efefef'.
		 * 
		 * @param string
		 * @return void
		 */
		function item_financeiro_organizar_cor_linha( destino )
		{
			var tabela = document.getElementById( 'tabela_financeiro_' + destino );
			var quantidade_tr = tabela.rows.length;
			var cor = '';
			for ( var i = 0; i < quantidade_tr; i++ )
			{
				var tr = tabela.rows[i];
				cor = cor == '' ? '#e9e9e9' : '' ;
				tr.style.backgroundColor = cor;
				tr.onmouseout = new Function( ' this.style.backgroundColor = "' + cor + '"; ' );
			}
		}
		
		/**
		 * Cria o conteúdo de uma célula da tabela de itens de despesa
		 * 
		 * @param string
		 * @param string
		 * @return object
		 */
		function item_financeiro_criar_input( valor, nome_campo )
		{
			var input = document.createElement( 'input' );
			input.type = 'hidden';
			input.name = nome_campo + '[]';
			input.value = valor;
			return input;
		}
		
		/**
		 * PReenche os campos do formulário de novo item financeiro com valores padrões.
		 * 
		 * @return void
		 */
		function item_financeiro_preencher_valores_default()
		{
			document.getElementById( 'novo_uso' ).value = '0';
			if ( financeiro_fontes_validas['105'] )
			{
				document.getElementById( 'nova_fonte' ).value = '105';
			}
			else if ( financeiro_fontes_validas_quantidade == 1 )
			{
				for ( var propriedade in financeiro_fontes_validas )
				{
					document.getElementById( 'nova_fonte' ).value = propriedade;
				}
			}
			document.getElementById( 'novo_oc' ).value = '9999';
		}
		
		/**
		 * Remove um item da tabela de financeiro.
		 * 
		 * @param string
		 * @param string
		 * @param boolean
		 * @return void
		 */
		function item_financeiro_remover( destino, id_tr, realizar_pergunta )
		{
			if ( realizar_pergunta )
			{
				if ( !confirm( 'Deseja realmente excluir o detalhamento?' ) )
				{
					return;
				}
			}
			houve_modificacao = true;
			var tr = document.getElementById( id_tr );
			var input = document.getElementById( id_tr + '_valor' );
			if ( tr && input )
			{
				item_financeiro_valor_total[destino] -= desformata_decimal( input.value );
				item_financeiro_atualizar_valor_total( destino );
				tr.parentNode.removeChild( tr );
				item_financeiro_organizar_cor_linha( destino );
			}
			//item_financeiro_atualizar_fisico();
		}
		
		// insere itens do financeiro existentes anteriores à exibição do formulário
		document.getElementById( 'item_financeiro_valor_total_suplementacao' ).value = '0';
		document.getElementById( 'item_financeiro_valor_total_cancelamento' ).value = '0';
		<? foreach ( $itens_financeiro as $item_financeiro ) : ?>
			item_financeiro_adicionar( 'cancelamento', '<?= $item_financeiro['natureza'] ?>', '<?= $item_financeiro['uso'] ?>', '<?= $item_financeiro['fonte'] ?>', '<?= $item_financeiro['oc'] ?>', '<?= $item_financeiro['valor'] ?>' );
		<? endforeach; ?>
		
		var spanValorTotalDespesa = document.getElementById( 'valor_total_despesa' );
		var valorAuxiliar = MascaraMonetario( item_financeiro_valor_total['total'] + '' );
		spanValorTotalDespesa.innerHTML = 'R$ ' + valorAuxiliar.substr( 0, valorAuxiliar.length - 3 );
		
		// preenche campos do formulario de item finacneiro com valores padrões
		item_financeiro_preencher_valores_default();
		document.getElementById( 'nova_natureza' ).focus();
		
	// FIM JAVASCRIPT FINANCEIRO
	
	/**
	 * Submete o formulário.
	 * 
	 * @return void
	 */
	function submeter_formulario()
	{
		document.formulario.aba_selecionada.value = aba_selecionada;
		document.formulario.submit();
	}
	
	/**
	 * Verifica se formulário este correto e o submete.
	 * 
	 * @return void
	 */
	function validar_cadastro()
	{
		var tot = item_financeiro_valor_total['total'];
		var totCan = item_financeiro_valor_total['cancelamento'];
		var totSup = item_financeiro_valor_total['suplementacao'];
		if ( totCan + totSup != total_antigo )
		{
			// erro! o valor inicial tem que bater com o valor antigo
			alert( 'O valor redistribuído não bate com o valor inicial das despesas.' );
			return;
		}
		document.formulario.act.value = 'gravar'; // altera status do formulário
		houve_modificacao = false; // evita que seja exibida um mensagem confirmando a submissão
		submeter_formulario();
	}
	
	/**
	 * Transforma uma string com vírgulas e pontos em um número real.
	 * 
	 * @param string
	 * @return float
	 */
	function desformata_decimal( valor )
	{
		if ( valor == null )
		{
			return 0.0;
		}
		valor = valor.replace( /\./g, '' );
		valor = valor.replace( ',', '.' );
		if ( valor[valor.length] == '.' )
		{
			valor += '0';
		}
		if ( valor.length == 0 )
		{
			return 0.0;
		}
		return parseFloat( valor );
	}
	
	// caso já tenha definidos os dados da ação os campos para escolha são bloqueados
	<? if ( $acaid ) : ?>
		bloquear_escolha_acao();
		document.getElementById( 'abas' ).style.display = 'block';
	<? endif; ?>
	
	/**
	 *  Abre o pop-up do relatório com os saldos por fonte na unidade cod = unidade
	 */
	function MostraRelatorio( cod )
	{
		window.open( "elabrev.php?modulo=principal/propostaorcamentaria/configuracao/mostraSaldosFontesUnidade&acao=A&unidade="+cod,'formulario','left=300,width=600,height=450,status=0,toolbar=0,location=0,scrollbars=1,resizable=1' );
	}

	/**
	 * Método chamada quanto o usuário quer sair da tela. Caso o
	 * formulário tenha sofrido alguma modificação um aviso é
	 * retornado.
	 * 
	 * @return string
	 */
	function sair_sem_salvar()
	{
		if ( houve_modificacao == true )
		{
			return 'Atenção. Existem dados do formulário que não foram guardados.';
		}
	}
	
	// define método para verificação de saída
	window.onbeforeunload = sair_sem_salvar;
	
</script>
