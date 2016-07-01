<?php
	
	/*
        Sistema Simec
        Setor responsável: SPO-MEC
        Desenvolvedor: Equipe Consultores Simec
        Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
        Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
        Módulo: despesadetalhamentosub.inc
        Finalidade: Manutenção na tabela despesaacao.
	*/
	
	/*
		Vários itens de despesa podem ser administrados nesta tela, porém
		apenas as de uma determinada ação. Os parâmetros para essa tela	pode
		ser o unicod, prgcod, acacod e loccod ou somente o acaid
	 */
	
	define( 'TIPO_PROPOSTA', 1 );
	define( 'TIPO_PROJETO_LEI', 2 );
	define( 'TIPO_LEI', 3 );

	// FUNÇÕES DA TELA
	
		/**
		 * Define as regras de permissoes de acesso as ugs
		 * 
		 * @return string
		 */

		function sql_consulta_ug() {
			
			global $db;
			
			$sql = "SELECT ung.ungcod as codigo, ung.ungdsc as descricao 
					FROM elabrev.usuarioresponsabilidade ur 
					INNER JOIN public.unidadegestora ung ON ung.ungcod=ur.ungcod 
					WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.rpustatus='A'";
			
			$existe = $db->pegaLinha($sql);
			
			if($existe) {
				return "SELECT ung.ungcod as codigo, ung.ungdsc as descricao 
						FROM elabrev.usuarioresponsabilidade ur 
						INNER JOIN public.unidadegestora ung ON ung.ungcod=ur.ungcod 
						WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.rpustatus='A'";
			} else {
				return "SELECT g.ungcod as codigo, g.ungdsc as descricao 
						FROM public.unidadegestora g WHERE g.ungstatus='A'";
			}
		}
		
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
		function atualizar_dados_acao( $acaid, $justificativa, $quantidade, $custo_total, $justificativaexpansao, $expansaoqtdfisico)
		{
			global $db;
			$acaid = (integer) $acaid;
			$quantidade = str_replace( array( '.', ',' ), '', $quantidade );
			$expansaoqtdfisico = str_replace( array( '.', ',' ), '', $expansaoqtdfisico );
			$custo_total = str_replace( array( '.', ',' ), '', $custo_total );
			//$justificativa = str_replace( "'", "\\'", $justificativa );
			$justificativa = simec_htmlspecialchars($justificativa, ENT_QUOTES);
			$justificativaexpansao = simec_htmlspecialchars($justificativaexpansao, ENT_QUOTES);
			
			$sql = "
				update elabrev.ppaacao_orcamento set
					justificativa = '" . $justificativa . "',
					justificativaexpansao = '" . $justificativaexpansao . "',
					acaqtdefisico = " . $quantidade . ",
                                        acaexpansaoqtdefisico = {$expansaoqtdfisico},
					acaqtdefinanceiro = " . $custo_total . "
				where
					prgano = '" . $_SESSION['exercicio'] . "' and
					acaid = " . $acaid;
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
			$sql = "
				select
					acaid
				from elabrev.ppaacao_orcamento
				where
					prgano = '" . $_SESSION['exercicio'] . "' and
					unicod = '" . $unicod . "' and
					prgcod = '" . $prgcod . "' and
					acacod = '" . $acacod . "' and
					loccod = '" . $loccod . "' ";
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
			$sql = "
				select
					ppaacao_orcamento.unmcod,
					ppaacao_orcamento.procod,
					ppaacao_orcamento.justificativa,
					ppaacao_orcamento.justificativaexpansao,
					ppaacao_orcamento.acaqtdefisico,
					ppaacao_orcamento.acaexpansaoqtdefisico,
					ppaacao_orcamento.acaqtdefinanceiro,
					ppaacao_orcamento.unicod,
					ppaacao_orcamento.unitpocod,
					ppaacao_orcamento.prgcod,
					ppaacao_orcamento.acacod,
					ppaacao_orcamento.acadsc,
					ppaacao_orcamento.loccod,
					unidade.unidsc,
					ppaprograma_orcamento.prgdsc,
					ppaacao_orcamento.sacdsc as locdsc,
					
					ppaacao_orcamento.acafinalidade,
					ppaacao_orcamento.acadescricao,
					ppaacao_orcamento.acabaselegal,
					ppaacao_orcamento.acadetalhamento
					
				from elabrev.ppaacao_orcamento
					inner join unidade using ( unicod )
					inner join elabrev.ppaprograma_orcamento using ( prgcod )
				where
					ppaacao_orcamento.prgano = '" . $_SESSION['exercicio'] . "' and
					acaid = " . $acaid;
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
		 * Captura ppodsc de uma proposta.
		 *
		 * @param integer $ppoid
		 */
		function pegar_dados_proposta( $ppoid )
		{
			global $db;
			$acaid = (integer) $acaid;
			$sql = "select ppodsc from elabrev.propostaorcamento where ppoid = " . $ppoid;
			$dados = $db->recuperar( $sql );
			if ( !$dados )
			{
				return array();
			}
			return $dados;
		}
		
		/**
		 * Captura as despesas (tabela elabrev.despesaacao ) a partir de um
		 * identificador de um ação e de uma proposta.
		 *
		 * @param integer $acaid
		 * @param integer $ppoid
		 * @return string[]
		 */
		function pegar_despesaacao( $acaid, $ppoid )
		{
			global $db;
			$acaid = (integer) $acaid;
			$ppoid = (integer) $ppoid;
			$sql = "select ungcod, sbacod, ndpcod, pl.plocodigo as ploid, iducod, foncod, idocod, dpavalor, dpavalorexpansao, ungcod 
					from elabrev.despesaacao d
						join idoc using ( idoid ) 
					    join naturezadespesa using ( ndpid ) 
					    left join elabrev.subacao using ( sbaid ) 
					    left join elabrev.planoorcamentario pl on pl.ploid = d.ploid 
					where d.acaid = " . $acaid . " and ppoid = " . $ppoid;
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
		 * Captura o sbaid a partir de um sbacod da tabela monitora.pi_subacao.
		 *
		 * @param string $sbacod
		 * @return integer
		 */
		function pegar_sbaid( $sbacod )
		{
			global $db;
			$sql = "select sbaid from elabrev.subacao where sbacod = '" . $sbacod . "' and unicod = '".$_REQUEST['unicod']."' and sbastatus = 'A'";
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
                 * Captura a descrição dop detalhe do produto.
                 *
                 * @param integer $acaid
                 * @return string
                 */
                function pegar_produto_detalhe($acaid) {
                    global $db;
                    $acaid = (integer) $acaid;
                    //$sql = "select prodsc from produto where procod = '" . $procod . "'";
                    $sql = "
                        select proddesc
                        from elabrev.ppaacao_orcamento
                        where acaid = '" . $acaid . "'
                        ";
                    return (string) $db->pegaUm($sql);
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
		 * @param integer $$ppoid
		 * @param integer $foncod
		 * @return void
		 */
		function remover_despesaacao( $acaid, $ppoid, $foncod = null )
		{
			global $db;
			$acaid = (integer) $acaid;
			$ppoid = (integer) $ppoid;
			$sql = "delete from elabrev.despesaacao where acaid = " . $acaid . " and ppoid = " . $ppoid;
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
				
				//$sql = "select tpdid, tpdcod from elabrev.tipodetalhamentoacao where acaid = " . $acaid . "";
				$sql = "select t.tpdid, t.tpdcod, ta.acaid from elabrev.tipodetalhamentoacao ta inner join elabrev.tipodetalhamento t on t.tpdid =  ta.tpdid where ta.acaid = " . $acaid . "";
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
		 * @param integer $ppoid
		 * @param string $ndpcod
		 * @param string $iducod
		 * @param string $foncod
		 * @param integer $idocod
		 * @param float $valor
		 * @return boolean
		 */
		function inserir_despesaacao( $acaid, $ppoid, $sbacod, $ploid, $ndpcod, $iducod, $foncod, $idocod, $valor, $ungcod, $valor_exp )
		{
			global $db;
			static $idoids = array();
			static $npdids = array();
			static $sbaids = array();
			$acaid = (integer) $acaid;
			$ppoid = (integer) $ppoid;
			$idoid = (integer) $idoid;
			$ploid = ($ploid ? $db->pegaUm("select ploid from elabrev.planoorcamentario where plocodigo = '$ploid' and acaid = '$acaid' ") : 'null');
			$valor = (float) $valor;
			$valor_exp = (float) $valor_exp;
			
			if ( !isset( $idoids[$idocod] ) )
			{
				$idoids[$idocod] = pegar_idoid( $idocod );
			}
			if ( !isset( $npdids[$ndpcod] ) )
			{
				$npdids[$ndpcod] = pegar_npdid( $ndpcod );
			}
			if ( !isset( $sbaids[$sbacod] ) )
			{
				$sbaids[$sbacod] = pegar_sbaid( $sbacod );
			}
			
			/*Executa a consulta para retornar o gnd da natureza da despesa*/
			$gnd = $ndpcod{1};
			
			/*Chama a função para retornar o tipo detalhamento que será incluído na despesa para a ação*/
			$tpdid = retorna_tipo_detalhamento($acaid, $gnd);
			if ( $tpdid === null ) $tpdid = 'null'; else $tpdid = "'" . $tpdid . "'";
			$sql = "insert into elabrev.despesaacao ( acaid, ppoid, sbaid, ndpid, iducod, foncod, idoid, dpavalor, tpdid, ungcod, ploid, dpavalorexpansao ) values ( " . $acaid . ", " . $ppoid . ", " . $sbaids[$sbacod] . ", " . $npdids[$ndpcod] . ", '" . $iducod . "', '" . $foncod . "', " . $idoids[$idocod] . ", " . $valor . " , " . $tpdid . ", '" . $ungcod . "', $ploid , " . $valor_exp . " ) ";

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
        	
                 /* Retirado do ar, Funcionando apenas para SU */
                $perfil = array();
                $perfil = pegaPerfilGeral( $_SESSION['usucpf'], $_SESSION['sisid'] );

                if (!in_array( PERFIL_SUPER_USUARIO, $perfil )) {
                    return false;
                }
                 /* FIM Retirado do ar, Funcionando apenas para SU */
    
                    //temporário para definição de momento posteriormente todo
			//return false;
			global $db;
			// captura o momento
			/*$sql =
				"select m.mtadsc " .
				" from elabrev.ppaacao_orcamento a " .
					" inner join elabrev.momentoacao m using ( mtaid ) " .
				" where " .
					" a.acaid = ".$acaid."";
			$mtadsc = trim( $db->pegaUm( $sql ) );*/
			$mtadsc = 'uo';
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
							" pflcod in (50,23,52) and ".
							" sisid = '2'";
					
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
			$acafinalidade   = '';
			$acadescricao    = '';
			$acabaselegal    = '';
			$acadetalhamento = '';
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
			$justificativaexpansao = '';
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
		
		// define identificador de proposta
		//$ppoid = (integer) $_REQUEST['ppoid'];
		$sql_proposta = "select ppoid from elabrev.propostaorcamento where ppostatus = 'A' and ppoanoexercicio = '" . $_SESSION['exercicio'] . "' and tppid = '". TIPO_PROPOSTA . "'";
		$ppoid = $db->pegaUm( $sql_proposta );
		// FIM define identificador de proposta
		
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
                                $expansao_fisico_quantidade = $dados_acao['acaexpansaoqtdefisico'];
				$fisico_total          = $dados_acao['acaqtdefinanceiro'];
				//$fisico_produto        = $dados_acao['procod'] . ' - ' . pegar_produto( $dados_acao['procod'] );
				$fisico_produto        = pegar_produto( $acaid );
                                $fisico_produto_detalhe = pegar_produto_detalhe($acaid);
				$fisico_unidade_medida = pegar_unidademedida( $acaid );
				
				$acafinalidade   = $dados_acao['acafinalidade'];
				$acadescricao    = $dados_acao['acadescricao'];
				$acabaselegal    = $dados_acao['acabaselegal'];
				$acadetalhamento = $dados_acao['acadetalhamento'];
				
			}
			else
			{
				$acaid = null;
			}
		}
		if ( $ppoid )
		{
			$dados_proposta = pegar_dados_proposta( $ppoid );
			$ppodsc = $dados_proposta['ppodsc'];
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
			- usuário acabou de escolher a proposta
			- usuário acabou de acessar a página e já está com a ação definida
		*/
		// carrega dados das abas da base de dados
		//if ( $act == 'finalizar_escolha_acao' || ( $act == null && $acaid ) )
		if ( $acaid && ( $act == 'finalizar_escolha_proposta' || $act == null ) )
		{
			// captura despesas da base de dados
			foreach ( pegar_despesaacao( $acaid, $ppoid ) as $dados_despesa )
			{
				$novo_item_financeiro = array(
					'ug' 	   => $dados_despesa['ungcod'],
					'subacao'  => $dados_despesa['sbacod'],
					'planoorcamentario'  => $dados_despesa['ploid'],
					'natureza' => $dados_despesa['ndpcod'],
					'uso'      => $dados_despesa['iducod'],
					'fonte'    => $dados_despesa['foncod'],
					'oc'       => $dados_despesa['idocod'],
					'valor'    => formata_numero( $dados_despesa['dpavalor'] ),
					'valor_exp'=> formata_numero( $dados_despesa['dpavalorexpansao'] )
				);
				array_push( $itens_financeiro, $novo_item_financeiro );
			}
			$dados_acao = pegar_dados_acao( $acaid );
			$justificativa = $dados_acao['justificativa'];
			$justificativaexpansao = $dados_acao['justificativaexpansao'];
			$fisico_quantidade = $dados_acao['acaqtdefisico'];
                        $expansao_fisico_quantidade = $dados_acao['acaexpansaoqtdefisico'];
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
						'ug'  	   => $_REQUEST['ug'][$indice_item_financeiro],
						'subacao'  => $_REQUEST['subacao'][$indice_item_financeiro],
						'planoorcamentario'  => $_REQUEST['planoorcamentario'][$indice_item_financeiro],
						'natureza' => $valor_natureza,
						'uso'      => $_REQUEST['uso'][$indice_item_financeiro],
						'fonte'    => $valor_fonte,
						'oc'       => $_REQUEST['oc'][$indice_item_financeiro],
						'valor'    => $_REQUEST['valor'][$indice_item_financeiro],
						'valor_exp'=> $_REQUEST['valor_exp'][$indice_item_financeiro],
                                                'valor_total' => $_REQUEST['valor_total'][$indice_item_financeiro]
					);
					array_push( $itens_financeiro, $novo_item_financeiro );
					//$valor_valor = str_replace( '.', '', $_REQUEST['valor'][$indice_item_financeiro] );
                                        $valor_total = str_replace('.', '', $_REQUEST['valor_total'][$indice_item_financeiro]);
//					$financeiro_total += $valor_valor;
					$financeiro_total += $valor_total;
					if ( !array_key_exists( $valor_fonte, $financeiro_total_por_fonte ) )
					{
						$financeiro_total_por_fonte[$valor_fonte] = 0;
					}
					//$financeiro_total_por_fonte[$valor_fonte] += $valor_valor;
					$financeiro_total_por_fonte[$valor_fonte] += $valor_total;
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
//                                ver($sql_verifica_saldo);
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
				$sql_distribuido_fonte_acao = "SELECT SUM(dpavalor+dpavalorexpansao) AS valor
                                                                 FROM elabrev.despesaacao
                                                                 WHERE foncod = '" . $financeiro_foncod . "'
                                                                   AND acaid = " . $acaid;
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
			$justificativaexpansao = $_REQUEST['justificativaexpansao'];
			$fisico_quantidade = $_REQUEST['fisico_quantidade'];
                        if ('' == $_REQUEST['expansao_fisico_quantidade']) {
                            $_REQUEST['expansao_fisico_quantidade'] = 0;
                        }
			$expansao_fisico_quantidade = $_REQUEST['expansao_fisico_quantidade'];
			$fisico_total = $_REQUEST['fisico_total'];
			// FIM alimenta variáveis da justifivativa e físico com os dados do formulário submetido
			// FIM alimenta variáveis do financeiro com os dados do formulário submetido

			if ( $verificacao_saldo == true )
			{
				// realiza requisições à base de dados
				$sucesso = false;
				if ( $acaid && $ppoid && $financeiro_total <= str_replace( '.', '', $fisico_total ) )
				{
					$sucesso = true;
					remover_despesaacao( $acaid, $ppoid );
					foreach ( $itens_financeiro as $item_financeiro )
					{
						$valor = str_replace( '.', '', $item_financeiro['valor'] );
						$valor_exp = str_replace( '.', '', $item_financeiro['valor_exp'] );
						if (!inserir_despesaacao(
                                                        $acaid,
                                                        $ppoid,
                                                        $item_financeiro['subacao'],
                                                        $item_financeiro['planoorcamentario'],
                                                        $item_financeiro['natureza'],
                                                        $item_financeiro['uso'],
                                                        $item_financeiro['fonte'],
                                                        $item_financeiro['oc'],
                                                        $valor,
                                                        $item_financeiro['ug'],
                                                        $valor_exp)
                                                ) {
							$sucesso = false;
							break;
						}
					}
					if( is_array($_POST['mpovalor']) ){
						$sql = "UPDATE elabrev.metaplanoorcamentario
                                                          SET mpostatus = 'I'
						          WHERE mpoid IN(SELECT mp.mpoid
                                                                           FROM elabrev.metaplanoorcamentario mp
								             INNER JOIN elabrev.planoorcamentario pl ON pl.ploid = mp.ploid
                                                                           WHERE pl.acaid = $acaid)";
						$db->executar( $sql );
						foreach ($_POST['mpovalor'] as $ploid => $mpovalor) {
							if ($ploid && ($mpovalor[0] || $_POST['mpovalorexpansao'][$ploid][0])){
								$mpovalor[0] = str_replace('.', '', $mpovalor[0]);
                                                                if (is_null($_POST['mpovalorexpansao'][$ploid][0]) || empty($_POST['mpovalorexpansao'][$ploid][0])) {
                                                                    $_POST['mpovalorexpansao'][$ploid][0] = 0;
                                                                }
                                                                
                                                                $_POST['mpovalorexpansao'][$ploid][0] = str_replace('.', '', $_POST['mpovalorexpansao'][$ploid][0]);
                                                                
								$sql = "INSERT INTO elabrev.metaplanoorcamentario(ploid, mpovalor, mpovalorexpansao)
								          VALUES ($ploid, $mpovalor[0], {$_POST['mpovalorexpansao'][$ploid][0]})";
                                                                $db->executar($sql);
							}
						}
					}
					if ( $sucesso == true )
					{
						$sucesso = atualizar_dados_acao( $acaid, $justificativa, $fisico_quantidade, $fisico_total, $justificativaexpansao, $expansao_fisico_quantidade);
					}
				}
				if ( $sucesso == true )
				{
					$db->commit();
					?>
					<script>
						alert("Operação realizada com sucesso!");
						location.href="elabrev.php?modulo=principal/propostaorcamentaria/despesadetalhamentosub&acao=A&acaid=<?=$acaid?>";
					</script>
					<?
					//header( "Location: elabrev.php?modulo=principal/propostaorcamentaria/despesadetalhamentosub&acao=A&acaid=" . $acaid );
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
	//$where_natureza = $habilita_gnd_1 ? "" : " and gndcod <> 1 and gndcod in ( select gndcod from elabrev.v_unidadegnd where unicod = '" . $unicod . "' ) ";
	
	// QUERYS PARA OS CAMPOS AJAX
		$sql_acao_unidade = "select distinct unidade.unicod as valor, unidade.unidsc as descricao from elabrev.ppaacao_orcamento join elabrev.usuarioresponsabilidade using ( unicod ) join unidade using ( unicod ) where unidade.unicod like '%s%' and unidade.unistatus = 'A' and unidade.unitpocod = 'U' and ppaacao_orcamento.acastatus = 'A' and acasnrap = false order by unidade.unicod";
		$sql_acao_programa = "select distinct ppaprograma_orcamento.prgcod as valor, ppaprograma_orcamento.prgdsc as descricao from elabrev.ppaacao_orcamento inner join unidade on ( ppaacao_orcamento.unicod= unidade.unicod ) inner join elabrev.ppaprograma_orcamento on ( ppaacao_orcamento.prgcod= ppaprograma_orcamento.prgcod ) where unidade.unicod like '%s%%' and unidade.unistatus = 'A' and unidade.unitpocod = 'U' and ppaprograma_orcamento.prgcod like '%s%%' and ppaprograma_orcamento.prgstatus = 'A' and ppaacao_orcamento.acastatus = 'A' and acasnrap = false order by ppaprograma_orcamento.prgcod";
		$sql_acao_acao = "select distinct ppaacao_orcamento.acacod as valor, ppaacao_orcamento.acadsc as descricao from elabrev.ppaacao_orcamento inner join unidade on ( ppaacao_orcamento.unicod= unidade.unicod ) inner join elabrev.ppaprograma_orcamento on ( ppaacao_orcamento.prgcod= ppaprograma_orcamento.prgcod ) where unidade.unicod like '%s%%' and unidade.unistatus = 'A' and unidade.unitpocod = 'U' and ppaprograma_orcamento.prgcod like '%s%%' and ppaprograma_orcamento.prgstatus = 'A' and upper( ppaacao_orcamento.acacod ) like upper( '%s%%' ) and ppaacao_orcamento.acastatus = 'A' and acasnrap = false order by ppaacao_orcamento.acacod";
		$sql_acao_localizador = "select distinct ppaacao_orcamento.loccod as valor, ppaacao_orcamento.sacdsc as descricao from elabrev.ppaacao_orcamento inner join unidade on ( ppaacao_orcamento.unicod= unidade.unicod ) inner join elabrev.ppaprograma_orcamento on ( ppaacao_orcamento.prgcod= ppaprograma_orcamento.prgcod ) where unidade.unicod like '%s%%' and unidade.unistatus = 'A' and unidade.unitpocod = 'U' and ppaprograma_orcamento.prgcod like '%s%%' and ppaprograma_orcamento.prgstatus = 'A' and upper( ppaacao_orcamento.acacod ) like upper( '%s%%' ) and ppaacao_orcamento.loccod like '%s%%' and ppaacao_orcamento.acastatus = 'A' and acasnrap = false order by ppaacao_orcamento.loccod";
		$sql_financeiro_subacao = "select s.sbacod as codigo, s.sbatitulo as descricao 
									from elabrev.subacao s
									INNER JOIN elabrev.subacaoprogramacao sa ON sa.sbaid = s.sbaid
									where sbastatus = 'A' and s.unicod = '".$unicod."'
									and sa.acacod = '".$acacod."' and sa.exercicio = '".$_SESSION['exercicio']."'
									group by s.sbacod, s.sbatitulo
									order by s.sbacod, s.sbatitulo";
		
		$sql_financeiro_planoorcamentario = "SELECT plocodigo AS codigo,
                                                            TRIM(plotitulo || ' | DETALHAMENTO: ' || plodetalhamento) AS descricao
                                                       FROM elabrev.planoorcamentario
                                                       WHERE acaid = $acaid
                                                         AND plostatus = 'A'
                                                       ORDER BY plocodigo";
		
		$sql_financeiro_ug = sql_consulta_ug();
		
		//$sql_financeiro_natureza = "select ndpcod as valor, ndpcod || ' - ' || ndpdsc as descricao from naturezadespesa where ndpcod like '%s%' and edpcod != '00' and ndpstatus = 'A' $where order by ndpcod";
		$sql_financeiro_natureza = "select ndpcod as codigo, ndpdsc as descricao from naturezadespesa where /*edpcod != '00' and*/ ndpstatus = 'A' " . $where_natureza . " AND ndpano = '{$_SESSION['exercicio']}' order by ndpcod";
		//$sql_financeiro_uso = "select iducod as valor, iducod || ' - ' || idudsc as descricao from identifuso where iducod like '%s%' and idustatus = 'A' order by iducod";
		$sql_financeiro_uso = "select iducod as codigo, idudsc as descricao from identifuso where idustatus = 'A' order by iducod";
		//$sql_financeiro_fonte = "select foncod as valor, foncod || ' - ' || fondsc as descricao from fonterecurso where foncod like '%s%' and fonstatus = 'A' and foncod in ( select foncod from elabrev.limitefonte where limitefonte.unicod = '" . $unicod . "' and limitefonte.unitpocod = '" . $unitpocod . "' ) order by foncod";
		$sql_financeiro_fonte = "select foncod as codigo, fondsc as descricao from public.fonterecurso where fonstatus = 'A' and foncod in ( select foncod from elabrev.limitefonte where limitefonte.unicod = '" . $unicod . "' and limitefonte.unitpocod = '" . $unitpocod . "' ) order by foncod";
		//$sql_financeiro_oc = "select idocod as valor, idocod || ' - ' || idodsc as descricao from idoc where idocod like '%s%' and idostatus = 'A' order by idodsc";
		$sql_financeiro_oc = "select idocod as codigo, idodsc as descricao from idoc where idostatus = 'A' order by idodsc";
	// FIM QUERYS PARA OS CAMPOS AJAX
	
	include APPRAIZ . "includes/cabecalho.inc";
?>
<style type="text/css">
.tabhdrfnc {
  border-right: 1px solid #c0c0c0 !important;
  border-bottom:1px solid #c0c0c0;
  border-left:1px solid #ffffff;
  text-align:center;
}
</style>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<script type="text/javascript" src="/includes/abas.js"></script>
<script type="text/javascript" language="javascript">

/**
 * Recebe um número e o retorno com divisores de ponto.
 */
function formata_numero(valor)
{
    valor = valor.toString();
    var valorRetorno = '';
    while (valor.length >= 3) {
        var tmp = valor.slice(-3);
        valor = valor.substr(0, valor.length - 3);
        if ('' === valorRetorno) {
            valorRetorno = tmp;
        } else {
            valorRetorno = tmp + '.' + valorRetorno;
        }
    }
    if ((valor.length) && (valorRetorno)) {
        valorRetorno = valor + '.' + valorRetorno;
    } else if (valor.length) {
        valorRetorno = valor;
    }
    return valorRetorno;
}

function soma_expansao()
{
    var novoValor = document.getElementById('novo_valor').value;
    var valorExpansao = document.getElementById('novo_valor_expansao').value;
    if (!novoValor) {
        novoValor = '0';
    }
    if (!valorExpansao) {
        valorExpansao = '0';
    }
    document.getElementById('novo_valor_total').value
        = formata_numero(parseInt(novoValor.replace(/\./g, '')) + parseInt(valorExpansao.replace(/\./g, '')));
}

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
		document.location.href = 'elabrev.php?modulo=principal/propostaorcamentaria/despesa&acao=A';
	}

	function selecionar_outra_proposta()
	{
		var acaid = document.getElementById( 'acaid' ).value;
		document.location.href = 'elabrev.php?modulo=principal/propostaorcamentaria/despesadetalhamentosub&acao=A&acaid=' + acaid;
	}
	
	function detalhar_ppa()
	{
		document.location.href = 'elabrev.php?modulo=principal/propostaorcamentaria/ppa&acao=A&acaid=' + <?= $acaid ?>;
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
	<input type="hidden" name="acaid" id="acaid" value="<?= $acaid ?>" />
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
			<td align="right" class="SubTituloDireita" >
				Ação:
			</td>
			<td>
				<? if ( $acaid ) : ?>
					<?= $acacod_descricao ?>
					<script type="text/javascript">
					
						function mostrarOcultarDetalhesAcao()
						{
							var tr_finalidade   = document.getElementById( 'acao_finalidade' );
							var tr_descricao    = document.getElementById( 'acao_descricao' );
							var tr_baselegal    = document.getElementById( 'acao_baselegal' );
							var tr_detalhamento = document.getElementById( 'acao_detalhamento' );
							if ( tr_finalidade.style.display != 'none' )
							{
								var visibilidade = 'none';
							}
							else
							{
								var visibilidade = 'table-row';
							}
							tr_finalidade.style.display   = visibilidade;
							tr_descricao.style.display    = visibilidade;
							tr_baselegal.style.display    = visibilidade;
							tr_detalhamento.style.display = visibilidade;
						}
					
					</script>
					(<a href="javascript:mostrarOcultarDetalhesAcao();">detalhar</a>)
					<input type="hidden" name="acacod" id="acacod" value="<?= $acacod ?>" />
				<? else : ?>
					<?= campo_texto_ajax_cascata( $sql_acao_acao, 'acacod', 'Ação', array( 'unicod', 'prgcod' ), 10, 4, '', 'left', false, $acacod, $acacod_descricao ) ?>
				<? endif; ?>
			</td>
		</tr>
		<? if ( $acaid ) : ?>
			<tr id="acao_finalidade" style="display: none;">
				<td width="20%" align="right" class="SubTituloDireita" >Finalidade da Ação:</td>
				<td width="80%"><?= $acafinalidade ?></td>
			</tr>
			<tr id="acao_descricao" style="display: none;">
				<td width="20%" align="right" class="SubTituloDireita" >Descrição da Ação:</td>
				<td width="80%"><?= $acadescricao ?></td>
			</tr>
			<tr id="acao_baselegal" style="display: none;">
				<td width="20%" align="right" class="SubTituloDireita" >Base legal da Ação:</td>
				<td width="80%"><?= $acabaselegal ?></td>
			</tr>
			<tr id="acao_detalhamento" style="display: none;">
				<td width="20%" align="right" class="SubTituloDireita" >Detalhamento da Ação:</td>
				<td width="80%"><?= $acadetalhamento ?></td>
			</tr>
		<? endif; ?>
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
                <?php
                /**
                 * Query de consulta adaptada de <mostraSaldoUnidade.inc>
                 * @see mostraSaldoUnidade.inc
                 */
                $query = <<<DML
SELECT SUM(COALESCE(vlLimite, 0)) AS vlLimite,
       SUM(COALESCE(vlDespesa, 0)) AS vlDespesa,
       SUM(COALESCE(vlLimite, 0)) - SUM(COALESCE(vlDespesa, 0)) AS vlSaldo
  FROM (SELECT SUM(COALESCE(lf.lmfvalor, 0)) AS vlLimite,
               0 AS vlDespesa
          FROM elabrev.limitefonte_calc2 lf
            INNER JOIN unidade u ON u.unicod = lf.unicod
              AND u.unitpocod = lf.unitpocod
            INNER JOIN elabrev.montantematriz mm ON mm.mtmid = lf.mtmid
            INNER JOIN elabrev.matriz m ON m.mtrid = mm.mtrid
            INNER JOIN elabrev.propostaorcamento po ON po.ppoid = mm.ppoid
              AND po.ppostatus = 'A'
              AND po.ppoanoexercicio = '{$_SESSION['exercicio']}'
            INNER JOIN public.fonterecurso fr ON fr.foncod = lf.foncod
            WHERE u.unicod = '{$unicod}'
        UNION ALL
        SELECT 0 AS vlLimite,
               sum(COALESCE(da.dpavalor + da.dpavalorexpansao, 0)) AS vlDespesa
          FROM elabrev.despesaacao da
          INNER JOIN elabrev.ppaacao_orcamento ac ON ac.acaid = da.acaid
          INNER JOIN public.fonterecurso fr2 ON fr2.foncod = da.foncod
          INNER JOIN elabrev.propostaorcamento po using(ppoid)
          WHERE ac.unicod = '{$unicod}'
            AND po.ppostatus = 'A'
            and po.ppoanoexercicio = '{$_SESSION['exercicio']}') AS foo
DML;
                $dinheiro = $db->pegaLinha($query);
                ?>
                <tr>
                    <td align="right" class="SubTituloDireita">Limite da Unidade:</td>
                    <td><?php echo 'R$ ' . (($dinheiro['vllimite'])?formata_numero($dinheiro['vllimite']):'0') . ',00'; ?></td><!-- (<a href="#">detalhar</a>) -->
                    </td>
                </tr>
<!--                <tr>
                    <td align="right" class="SubTituloDireita">Saldo disponível:</td>
                    <td><?php echo 'R$ ' . (($dinheiro['vlsaldo'])?'<span id="vlsaldo">'.formata_numero($dinheiro['vlsaldo']):'0') . '</span>,00'; ?></td>
                    </td>
                </tr>-->
		<? if ( $acaid ) : ?>
			<tr>
				<td align="right" class="SubTituloDireita" >&nbsp;</td>
				<td>
					<input type="button" name="botao_selecionar_outra_acao" value="Selecionar outra Ação" onclick="selecionar_outra_acao();" />
				</td>
			</tr>
			<?php if ( $fisico_produto != "-" && $fisico_unidade_medida != "-" ) : ?>
				<tr>
					<td align="right" class="SubTituloDireita" >&nbsp;</td>
					<td>
						<input
							type="button"
							name="botao_detalhar_ppa"
							value="Detalhar PPA"
							onclick="detalhar_ppa();"
						/>
					</td>
				</tr>
			<? endif; ?>
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
		
		// JAVASCRIPT PROPOSTA
		
			/**
			 * Finaliza a escolha da proposta. Todos os campos utilizados para
			 * a escolha da proposta são desabilitados.
			 * 
			 * @return void
			 */
			function finalizar_escolha_proposta()
			{
				// altera status do formulário
				document.formulario.act.value = 'finalizar_escolha_proposta';
				submeter_formulario();
			}
			
			function bloquear_escolha_proposta(){}
			
		// FIM JAVASCRIPT PROPOSTA
		
	</script>
	<p id="botao_finalizar_acao" style="text-align: center; <?= $acaid ? 'display: none;' : '' ?>">
		<input type="button" name="finalizar_acao" value="Finalizar Escolha da Ação" onclick="javascript:submeter_escolha_acao();" />
	</p>
	<div id="abas" style="display: <?= $acaid && $ppoid ? 'block' : 'none' ?>;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Financeiro</li>
			</ul>
		</div>
		<?
		$total = $db->pegaUm( "select count(plocodigo) from elabrev.planoorcamentario where acaid = $acaid" );
		$boPlocodigo = ($total > 0 ? 'true' : 'false');
		
		$total = $db->pegaUm( "select count(ploid) from elabrev.planoorcamentario where ploproduto is not null and acaid = $acaid" );
		$boMetas = ($total > 0 ? 'true' : 'false');
		?>
		<!-- FINANCEIRO -->
		<input type="hidden" value="<?=$boPlocodigo; ?>" id="boPlocodigo" name="boPlocodigo">
		<input type="hidden" value="<?=$boMetas; ?>" id="boMetas" name="boMetas">
		<div id="aba_financeiro" class="conteudoAba">
                    <table align="center" border="0" cellspacing="0" cellpadding="2" width="80%">
                        <thead>
                            <tr>
                                <td style="text-align:right;padding-right:30px;font-size:12px">
                                    Saldo da Unidade: R$
                                    <span id="saldo_disponivel">
                                        <?php
                                        $ultimoValorInserido = 0;
                                        if (isset($_REQUEST['valor_total'])) {
                                            $ultimoValorInserido = $_REQUEST['valor_total'][count($_REQUEST['valor_total']) - 1];
                                        }
                                        echo (($dinheiro['vlsaldo'])
                                                ?formata_numero($dinheiro['vlsaldo'] - $ultimoValorInserido)
                                                :'0');
                                        ?>
                                    </span>,00
                                </td>
                            </tr>
                        </thead>
                    </table>
			<table align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" width="80%">
                            <thead>
                                <tr>
                                    <td width="90" valign="middle" class="title tabhdrfnc" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>UG</strong></td>
                                    <td width="90" valign="middle" class="title tabhdrfnc" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Subação</strong></td>
                                    <?if( $boPlocodigo == 'true' ){ ?>
                                    <td width="90" valign="middle" class="title tabhdrfnc" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Plano Orçamentário</strong></td>
                                    <?} ?>						
                                    <td width="90" valign="middle" class="title tabhdrfnc" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Natureza</strong></td>
                                    <td width="60" valign="middle" class="title tabhdrfnc" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IDUSO</strong></td>
                                    <td width="75" valign="middle" class="title tabhdrfnc" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Fonte</strong></td>
                                    <td width="75" valign="middle" class="title tabhdrfnc" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IDOC</strong></td>
                                    <td width="155" valign="middle" class="title tabhdrfnc" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor (R$ 1,00)</strong></td>
                                    <td width="155" valign="middle" class="title tabhdrfnc" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Limite Adicional (R$ 1,00)</strong></td>
                                    <td width="155" valign="middle" class="title tabhdrfnc" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Total<br />(Valor + Limite Adicional)</strong></td>
                                    <td width="65" valign="middle" class="title tabhdrfnc" style="border-right:1px white solid !important" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
                                </tr>
                            </thead>
				<? if ( $permissao_momento ) : ?>
					<tr bgcolor="#f7f7f7">
						<td>
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'ug\' );" '; ?>
							<? 
							$ugs_existente = $db->carregar($sql_financeiro_ug);
							if($ugs_existente[0] && count($ugs_existente) == 1):
							?>
							<input type="text" value="<?=$ugs_existente[0]['codigo']; ?>" class="disabled" readonly="readonly" id="nova_ug" name="nova_ug" size="10">
							<? else: ?>
							<? texto_popup( 'nova_ug', $sql_financeiro_ug, 'Subação', 8, 8, '', '', $complemento ); ?>
							<? endif; ?>
						</td>

						<td>
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'subacao\' );" '; ?>
							<? 
							$subs_existente = $db->carregar($sql_financeiro_subacao);
							if($subs_existente[0] && count($subs_existente) == 1):
							?>
							<input type="text" value="<?=$subs_existente[0]['codigo']; ?>" class="disabled" readonly="readonly" id="nova_subacao" name="nova_subacao" size="10">
							<? else: ?>
							<? texto_popup( 'nova_subacao', $sql_financeiro_subacao, 'Subação', 8, 8, '', '', $complemento ); ?>
							<? endif; ?>
						</td>
						<?if( $boPlocodigo == 'true' ){ ?>
							<td>
								<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'planoorcamentario\' );" '; ?>
								<?
								$plo_existente = $db->carregar($sql_financeiro_planoorcamentario);
								if($plo_existente[0] && count($plo_existente) == 1):
								?>
								<input type="text" value="<?=$plo_existente[0]['codigo']; ?>" class="disabled" readonly="readonly" id="nova_planoorcamentario" name="nova_planoorcamentario" size="10">
								<? else: ?>
								<? texto_popup( 'nova_planoorcamentario', $sql_financeiro_planoorcamentario, 'Plano Orcamentário', 8, 8, '', '', $complemento ); ?>
								<? endif; ?>
							</td>
						<?} else { ?>
							<input type="hidden" name="nova_planoorcamentario[]" id="nova_planoorcamentario" value="">
						<?} ?>
						<td>
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'natureza\' );" '; ?>
							<? texto_popup( 'nova_natureza', $sql_financeiro_natureza, 'Natureza da Despesa', 8, 8, '########', '', $complemento ); ?>
						</td>
						<td>
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'iduso\' );" '; ?>
							<? texto_popup( 'novo_uso', $sql_financeiro_uso, 'Ident. de Uso', 1, 3, '#', '', $complemento ); ?>
						</td>
						<td>
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'fonte\' );" '; ?>
							<? texto_popup( 'nova_fonte', $sql_financeiro_fonte, 'Fonte de Recursos', 3, 5, '###', '', $complemento ); ?>
						</td>
						<td>
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'idoc\' );" '; ?>
							<? texto_popup( 'novo_oc', $sql_financeiro_oc, 'Ident. Op. de Crédito', 4, 6, '####', '', $complemento ); ?>
						</td>
						<td align="right">
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'valor\' );" '; ?>
							<?= campo_texto( 'novo_valor', 'N', 'S', '', 18, 14, '##.###.###.###', '', 'right', '', 0, ' id="novo_valor" ' . $complemento, null, null, 'soma_expansao()'); ?>
						</td>
						<td align="right">
							<? $complemento = ''; ?>
							<?= campo_texto( 'novo_valor_expansao', 'N', 'S', '', 18, 14, '##.###.###.###', '', 'right', '', 0, ' id="novo_valor_expansao" ' . $complemento, null, null, 'soma_expansao()'); ?>
						</td>
						<td align="right">
							<? $complemento = ''; ?>
							<?= campo_texto( 'novo_valor_total', 'N', 'N', '', 18, 14, '##.###.###.###', '', 'right', '', 0, ' id="novo_valor_total" onblur="MouseBlur(this);" ' . $complemento); ?>
						</td>
						<td align="left">
							<a href="javascript:item_financeiro_adicionar_do_formulario();" title="Incluir" style="margin: 0; padding: 0 6px;">
								<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
							</a>
						</td>
					</tr>
				<? else : ?>
					<input type="hidden" value="" name="nova_uo" id="nova_uo"/>
					<input type="hidden" value="" name="nova_subacao" id="nova_subacao"/>
					<input type="hidden" value="" name="nova_planoorcamentario" id="nova_planoorcamentario"/>
					<input type="hidden" value="" name="nova_natureza" id="nova_natureza"/>
					<input type="hidden" value="" name="novo_uso" id="novo_uso"/>
					<input type="hidden" value="" name="nova_fonte" id="nova_fonte"/>
					<input type="hidden" value="" name="novo_oc" id="novo_oc"/>
					<input type="hidden" value="" name="novo_valor" id="novo_valor"/>
					<input type="hidden" value="" name="novo_valor_expansao" id="novo_valor_expansao"/>
				<? endif; ?>
				<tr>
					<td colspan="<?=($boPlocodigo == 'true' ? '11' : '10'); ?>">
						<div class="div_rolagem" id="div_tabela_financeiro" style="width:100%; padding: 0; margin: 0; height: 100px;position-y: absolute">
							<table id="tabela_financeiro" border="0" cellspacing="0" cellpadding="2" width="100%">
								<tr valign="bottom">
									<td width="92" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="93" style="height: 0; margin: 0; padding: 0;"></td>
									<?if( $boPlocodigo == 'true' ){ ?>
										<td width="93" style="height: 0; margin: 0; padding: 0;"></td>
									<?} ?>
									<td width="95" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="65" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="78" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="78" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="160" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="160" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="160" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="23" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="25" style="height: 0; margin: 0; padding: 0;"></td>
								</tr>
							</table>
						</div>
					</td>
				</tr>
				<tr valign="bottom">
					<td align="right" colspan="5" style="color: #ababab; border-top: 3px solid #dfdfdf;">Total</td>
					<td align="right" colspan="3"style="color: #0066cc; border-top: 3px solid #dfdfdf;" valign="bottom">
						<?= campo_texto( 'item_financeiro_valor_total', 'N', 'N', 'Valor Total', 25, 20, '', '', 'right', '', 0, ' id="item_financeiro_valor_total" ' ) ?>
					</td>
					<td style="border-top: 3px solid #dfdfdf;" colspan="2">&nbsp;</td>
				</tr>
				<tr valign="bottom">
					<td align="right" colspan="10" style="border-top: 3px solid #dfdfdf;">
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
	<div id="abas" style="display: <?=($boMetas == 'true' ? 'block' : 'none')?>;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Metas</li>
			</ul>
		</div>
		<!-- Metas -->
		<?
		$sql = "SELECT
					pa.plocodigo, 
					pa.plocodigo || ' - ' || pa.plotitulo as descricao, 
					pa.ploid, 
					mp.mpovalor,
                                        mp.mpovalorexpansao,
					pa.ploproduto,
					pa.plounidademedida  
				FROM 
					elabrev.planoorcamentario pa
				left join 
					elabrev.metaplanoorcamentario mp on mp.ploid = pa.ploid	and mp.mpostatus = 'A'
				WHERE 
					pa.acaid = $acaid
					and pa.plostatus = 'A'
					and pa.ploproduto is not null 
				order by 
					pa.plocodigo";
//		dbg($sql); die();
		$arrPlano = $db->carregar( $sql );
		$arrPlano = $arrPlano ? $arrPlano : array();
		?>
		<div id="aba_metas" class="conteudoAba">
			<table class="listagem" cellpadding="2" width="70%" cellspacing="0" border="0" align="center">
				<thead>
					<tr align="center">
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Plano Orçamentário</strong></td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Produto</strong></td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Unidade de Medida</strong></td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Quantidade</strong></td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Limite Adicional</strong></td>
					</tr>
				</thead>
				<?php $x=1; ?>
				<?foreach ($arrPlano as $key => $valor) { ?>
				<?php $corX = $x % 2 ? '#F7F7F7' : ''; $x++; ?>
				<tr align="center" bgcolor="<?php echo $corX; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $corX; ?>';" id="tr_po_<?php echo $valor['plocodigo']; ?>" style="display:none;">
					<? $habilitado = $permissao_momento ? 'S' : 'N' ; ?>
					<td width="50%" style="text-align: left;"><?=$valor['descricao']; ?></td>
					<td width="25%" style="text-align: left;"><?=$valor['ploproduto']; ?></td>
					<td width="15%" style="text-align: left;"><?=$valor['plounidademedida']; ?></td>
					<td width="10%" style="text-align: left;">
						<input id="mpovalor_<?php echo $valor['plocodigo']; ?>" class=" normal" type="text" alt="<?php echo $valor['ploproduto']; ?>" title="Quantidade" onfocus="MouseClick(this);this.select();" 
							onkeyup="this.value=mascaraglobal( '###.###.###', this.value );" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
							onmouseover="MouseOver(this);" value="<?=formata_numero($valor['mpovalor']); ?>" maxlength="11" size="21" name="mpovalor[<?=$valor['ploid']; ?>][]" style="text-align:right;">
					</td>
					<td width="10%" style="text-align: left;">
						<input id="mpovalorexpansao_<?php echo $valor['plocodigo']; ?>" class=" normal" type="text" alt="<?php echo $valor['ploproduto']; ?>" title="Limite Adicional" onfocus="MouseClick(this);this.select();" 
							onkeyup="this.value=mascaraglobal( '###.###.###', this.value );" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
							onmouseover="MouseOver(this);" value="<?=formata_numero($valor['mpovalorexpansao']); ?>" maxlength="11" size="21" name="mpovalorexpansao[<?=$valor['ploid']; ?>][]" style="text-align:right;">
					</td>
				</tr>
				<?} ?>
			</table>
			<? if ( $permissao_momento ) : ?>
				<br/><br/>
				<input type="button" name="salvar" value="Salvar" onclick="javascript:validar_cadastro();"/>
			<? endif; ?>
		</div>
	</div>
	<div id="abas" style="display: <?= $acaid && $ppoid ? 'block' : 'none' ?>;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Físico</li>
			</ul>
		</div>
		<!-- FÍSICO -->
		<div id="aba_fisico" class="conteudoAba">
			<table cellpadding="2" width="70%" cellspacing="0" border="0" align="center">
				<tr align="left">
					<td width="20%" style="border-bottom: 1px solid #cdcdcd;"><strong>Produto:</strong></td>
					<td style="border-bottom: 1px solid #cdcdcd;"><?= $fisico_produto ?></td>
					<td style="border-bottom: 1px solid #cdcdcd;"><strong>Unidade de Medida:</strong></td>
					<td style="border-bottom: 1px solid #cdcdcd;"><?= $fisico_unidade_medida ?></td>
				</tr>
			</table>
            <table cellpadding="2" width="70%" cellspacing="0" border="0" align="center">
                <tr align="left">
                    <td style="border-bottom: 1px solid #cdcdcd; text-justify: inter-cluster;">
                        <b>Especificação do produto: </b> <? echo $fisico_produto_detalhe; ?>
                    </td>
                </tr>
            </table>
			<br/>
			<table class="listagem" cellpadding="2" width="70%" cellspacing="0" border="0" align="center">
				<thead>
					<tr align="center">
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Quantidade</strong></td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custo Médio</strong></td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Limite Adicional</strong></td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custo Total</strong></td>
					</tr>
				</thead>
				<tr align="center">
					<? $habilitado = $permissao_momento ? 'S' : 'N' ; ?>
					<td width="10%">Proposta</td>
					<td width="30%">
						<? $complemento = !$permissao_momento ? ' id="fisico_quantidade" ' : ' onkeyup="this.value=mascaraglobal( \'###.###.###\', this.value ); altera_fisico_quantidade();" id="fisico_quantidade" onblur="MouseBlur(this);" '; ?>
						<?= campo_texto( 'fisico_quantidade', 'N', $habilitado, 'Quantidade', 20, 11, '', '', 'right', '', 0, $complemento ) ?>
					</td>
					<td width="30%">
						<? $complemento = !$permissao_momento ? ' id="fisico_media" ' : ' onkeyup="this.value=mascaraglobal( \'#.###.###.###\', this.value ); altera_fisico_media();" id="fisico_media" onblur="MouseBlur(this);" '; ?>
						<?= campo_texto( 'fisico_media', 'N', $habilitado, 'Quantidade', 20, 13, '', '', 'right', '', 0, $complemento ) ?>
					</td>
					<td width="30%">
						<? $complemento = !$permissao_momento ? ' id="expansao_fisico_quantidade" ' : ' onkeyup="this.value=mascaraglobal( \'#.###.###.###\', this.value );" id="expansao_fisico_quantidade" onblur="MouseBlur(this);" '; ?>
						<?= campo_texto( 'expansao_fisico_quantidade', 'N', $habilitado, 'Limite Adicional', 20, 13, '', '', 'right', '', 0, $complemento ) ?>
					</td>
					<td width="30%">
						<? $complemento = !$permissao_momento ? ' id="fisico_total" ' : ' onkeyup="this.value=mascaraglobal( \'###.###.###.###\', this.value ); altera_fisico_total();" id="fisico_total" onblur="MouseBlur(this);" '; ?>
						<?= campo_texto( 'fisico_total', 'N', $habilitado, 'Quantidade', 20, 15, '', '', 'right', '', 0, $complemento ) ?>
					</td>
				</tr>
			</table>
			<? if ( $permissao_momento ) : ?>
				<br/><br/>
				<input type="button" name="salvar" value="Salvar" onclick="javascript:validar_cadastro();"/>
			<? endif; ?>
		</div>
	</div>
	<div id="abas" style="display: <?= $acaid && $ppoid ? 'block' : 'none' ?>;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Justificativa</li>
			</ul>
		</div>
		<!-- JUSTIFICATIVA -->
		<div align="center" id="aba_justificativa" class="conteudoAba">
			<? $habilitado = $permissao_momento ? 'S' : 'N' ; ?>
				<table align="center" border="0"><tr><td>
				<?= campo_textarea( 'justificativa', 'S', $habilitado, 'Justificativa', 100, 3, 500, ' onkeypress="altera_justificativa( event );" ' ); ?>
				</td></tr></table>
				
			<? if ( $permissao_momento ) : ?>
				<br/><br/>
				<input type="button" name="salvar" value="Salvar" onclick="javascript:validar_cadastro();"/>
			<? endif; ?>
		</div>
	</div>
	
	<div id="abas" style="display: <?= $acaid && $ppoid ? 'block' : 'none' ?>;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Just. Limite Adicional</li>
			</ul>
		</div>
		<!-- JUSTIFICATIVA DA EXPANSÃO -->
		<div align="center" id="aba_justificativa_expansao" class="conteudoAba">
			<? $habilitado = $permissao_momento ? 'S' : 'N' ; ?>
				<table align="center" border="0"><tr><td>
				<?= campo_textarea( 'justificativaexpansao', 'N', $habilitado, 'Justificativa do Limite Adicional', 100, 3, 500, ' onkeypress="altera_justificativa_expansao( event );" ' ); ?>
				</td></tr></table>
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

	// JAVASCRIPT JUSTIFICATIVA
	
		/**
		 * Captura o evento e verifica se houve modificação no campo justificativa.
		 * 
		 * @param event
		 * @return void
		 */
		function altera_justificativa( event )
		{
			var keyCode = event.keyCode;
			// ignora a tecla 'esc', setas (esquerda, cima, direita e baixo)
			if ( keyCode != 37 && keyCode != 38 && keyCode != 39 && keyCode != 40 && keyCode != 27 )
			{
				houve_modificacao = true;
			}
		}
		
		function altera_justificativa_expansao( event )
		{
			var keyCode = event.keyCode;
			// ignora a tecla 'esc', setas (esquerda, cima, direita e baixo)
			if ( keyCode != 37 && keyCode != 38 && keyCode != 39 && keyCode != 40 && keyCode != 27 )
			{
				houve_modificacao = true;
			}
		}
	
	// FIM JAVASCRIPT JUSTIFICATIVA
	
	// JAVASCRIPT FÍSICO
		
		/**
		 * ...
		 * 
		 * @param integer
		 * @param integer
		 * @return void
		 */
		function atualiza_fisico( quantidade, total )
		{
			quantidade = MascaraMonetario( quantidade + '' ).split( ',' );
			quantidade = quantidade[0];
			total = MascaraMonetario( total + '' ).split( ',' );
			total = total[0];
			document.getElementById( 'fisico_quantidade' ).value = quantidade;
			document.getElementById( 'fisico_total' ).value = total;
			altera_fisico_quantidade();
		}
		
		function altera_fisico_total()
		{
			houve_modificacao = true;
			var input_quantidade = document.getElementById( 'fisico_quantidade' );
			var input_media = document.getElementById( 'fisico_media' );
			var input_total = document.getElementById( 'fisico_total' );
			if ( !input_quantidade.value )
			{
				input_media.value = '';
				input_total.value = '';
				alert( 'Preencha o campo quantidade.' );
				input_quantidade.focus();
				return;
			}
			if ( !input_total.value )
			{
				input_media.value = '';
				return;
			}
			var valor_total = parseInt( input_total.value.replace( /\./g, '' ) );
			var valor_quantidade = parseInt( input_quantidade.value.replace( /\./g, '' ) );
			var valor_media = MascaraMonetario( ( valor_total / valor_quantidade ) + '' );
			valor_media = valor_media.split( ',' );
			input_media.value = valor_media[0];
		}
		
		function altera_fisico_media()
		{
//			houve_modificacao = true;
//			var input_quantidade = document.getElementById( 'fisico_quantidade' );
//			var input_media = document.getElementById( 'fisico_media' );
//			var input_total = document.getElementById( 'fisico_total' );
//			if ( !input_quantidade.value )
//			{
//				input_media.value = '';
//				input_total.value = '';
//				alert( 'Preencha o campo quantidade.' );
//				input_quantidade.focus();
//				return;
//			}
//			if ( !input_media.value )
//			{
//				input_total.value = '';
//				return;
//			}
//			var valor_media = parseInt( input_media.value.replace( /\./g, '' ) );
//			var valor_quantidade = parseInt( input_quantidade.value.replace( /\./g, '' ) );
//			var valor_total = MascaraMonetario( ( valor_media * valor_quantidade ) + '' );
//			valor_total = valor_total.split( ',' );
//			input_total.value = valor_total[0];
		}
		
		function altera_fisico_quantidade()
		{
//			houve_modificacao = true;
//			var input_quantidade = document.getElementById( 'fisico_quantidade' );
//			var input_media = document.getElementById( 'fisico_media' );
//			var input_total = document.getElementById( 'fisico_total' );
//			if ( input_quantidade.value == '' || input_quantidade.value == '0' )
//			{
//				input_media.value = '';
//				input_total.value = '';
//				return;
//			}
//			if ( input_total.value != '' )
//			{
//				var valor_total = parseInt( input_total.value.replace( /\./g, '' ) );
//				var valor_quantidade = parseInt( input_quantidade.value.replace( /\./g, '' ) );
//				var valor_media = MascaraMonetario( ( valor_total / valor_quantidade ) + '' );
//				valor_media = valor_media.split( ',' );
//				input_media.value = valor_media[0];
//			}
		}
		
		// atualiza físico com dados do banco
		<? if ( $fisico_quantidade && $fisico_total ) : ?>
			atualiza_fisico( <?= str_replace( array( ',', '.' ), '', $fisico_quantidade ) ?>, <?= str_replace( array( ',', '.' ), '', $fisico_total ) ?> );
			houve_modificacao = false;
		<? endif; ?>
	
	// FIM JAVASCRIPT FÍSICO
	
	// JAVASCRIPT FINANCEIRO
		
		/**
		 * Contém os itens financeiros adicionados. Utilizado para
		 * evitar repetições de subacao-natureza-iduso-fonte-idoc na tabela
		 * de item financeiro.
		 * 
		 * @var boolean[]
		 */
		var item_financeiros_adicionados = new Array();
		
		/**
		 * Soma de todos os itens da tabela financeiro.
		 * 
		 * @var float
		 */
		var item_financeiro_valor_total = 0;
		
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
		var item_financeiro_id_atual = 0;
		
		/**
		 * Indica se existia itens financeiros ao iniciar a tela.
		 * 
		 * @var float
		 */
		var possui_item_financeiro_inicial = <?= count( $itens_financeiro ) ? 'true' : 'false' ?>;
		
		var financeiro_ug_validas = new Array();
		<?
			$dados_temp = $db->carregar( $sql_financeiro_ug );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?> financeiro_ug_validas['<?= $dados_temp_valor['codigo'] ?>'] = true; <?
				}
			}
		?>

		
		// VALORES DOS CAMPOS ITEM FINANCEIRO VÁLIDOS
		var financeiro_subacao_validas = new Array();
		<?
			$dados_temp = $db->carregar( $sql_financeiro_subacao );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?> financeiro_subacao_validas['<?= $dados_temp_valor['codigo'] ?>'] = true; <?
				}
			}
		?>
		
		var financeiro_planoorcamentario_validas = new Array();
		<?
			$dados_temp = $db->carregar( $sql_financeiro_planoorcamentario );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?> financeiro_planoorcamentario_validas['<?= $dados_temp_valor['codigo'] ?>'] = true; <?
				}
			}
		?>
		
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
		 * @return boolean
		 */
		function item_financeiro_adicionar( ug, subacao, planoorcamentario, natureza, uso, fonte, oc, valor, valor_exp )
		{
			
//			if ( valor <= 0 )
//			{
//				alert( 'Valor deve ser maior que zero.' );
//				document.getElementById( 'novo_valor' ).focus();
//				return false;
//			}
			if( planoorcamentario == '0' ) planoorcamentario = '';
			
			var id_novo_item = ug + subacao + planoorcamentario + natureza + uso + fonte + oc;
			// verifica se item já existe
			if ( item_financeiros_adicionados[id_novo_item] )
			{
				alert( 'Já existe item financeiro para esta subação, natureza, fonte, iduso e idoc. Você pode alterar o valor desse item selecionando o item correspondente na tabela abaixo do formulário.' );
				// posiciona a linha a ser destacada
				document.getElementById( item_financeiros_adicionados[id_novo_item] + '_ancora' ).focus();
				var funcao_hilight = new Function( ' item_financeiro_hilight( \'' + item_financeiros_adicionados[id_novo_item] + '\', 30 ); ' );
				setTimeout( funcao_hilight, 300 );
				return false;
				
			}
			// define dados gerais do item a ser adicionado
			item_financeiro_id_atual++;
			
			var tabela = document.getElementById( 'tabela_financeiro' );
			var id = 'item_financeiro_' + item_financeiro_id_atual;
			item_financeiros_adicionados[id_novo_item] = id;
			// FIM define dados gerais do item a ser adicionado
			// cria nova linha para a tabela de financeiro
			var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
			    nova_linha.id = id;
			    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
			// FIM cria nova linha para a tabela de financeiro
			// cria as célular da nova linha
			if( document.getElementById( 'boPlocodigo' ).value == 'true' ){
				var celula_ug = nova_linha.insertCell( 0 );
				var celula_subacao = nova_linha.insertCell( 1 );
				var celula_planoorcamentario = nova_linha.insertCell( 2 );
				var celula_natureza = nova_linha.insertCell( 3 );
				var celula_uso = nova_linha.insertCell( 4 );
				var celula_fonte = nova_linha.insertCell( 5 );
				var celula_oc = nova_linha.insertCell( 6 );
				var celula_valor = nova_linha.insertCell( 7 );
				    celula_valor.style.textAlign = 'right';
				    celula_valor.style.color = '#0066cc';
				    
				var celula_valor_exp = nova_linha.insertCell( 8 );
				    celula_valor_exp.style.textAlign = 'right';
				    celula_valor_exp.style.color = '#0066cc';
				var celula_valor_total = nova_linha.insertCell(9);
				    celula_valor_total.style.textAlign = 'right';
				    celula_valor_total.style.color = '#0066cc';
				var celula_acao_alterar = nova_linha.insertCell( 10 );
				    celula_acao_alterar.style.textAlign = 'center';
				var celula_acao_remover = nova_linha.insertCell( 11 );
				    celula_acao_remover.style.textAlign = 'center';
				// FIM cria as célular da nova linha
				// cria campos ocultos com os dados do novo item
				var input_hidden_ug = item_financeiro_criar_input( ug, 'ug' );
				var input_hidden_subacao = item_financeiro_criar_input( subacao, 'subacao' );
				var input_hidden_planoorcamentario = item_financeiro_criar_input( planoorcamentario, 'planoorcamentario' );
				var input_hidden_natureza = item_financeiro_criar_input( natureza, 'natureza' );
				var input_hidden_uso = item_financeiro_criar_input( uso, 'uso' );
				var input_hidden_fonte = item_financeiro_criar_input( fonte, 'fonte' );
				var input_hidden_oc = item_financeiro_criar_input( oc, 'oc' );
				var input_hidden_valor = item_financeiro_criar_input( valor, 'valor' );
					input_hidden_valor.id = id + '_valor';
				var input_hidden_valor_exp = item_financeiro_criar_input( valor_exp, 'valor_exp' );
					input_hidden_valor_exp.id = id + '_valor_exp';
                                var input_hidden_total = item_financeiro_criar_input(parseInt(valor.replace(/\./g, ''))
                                    + parseInt(valor_exp.replace(/\./g, '')), 'valor_total');
                                input_hidden_total.id = id + '_valor_total';
			} else {
				var celula_ug = nova_linha.insertCell( 0 );
				var celula_subacao = nova_linha.insertCell( 1 );
				var celula_natureza = nova_linha.insertCell( 2 );
				var celula_uso = nova_linha.insertCell( 3 );
				var celula_fonte = nova_linha.insertCell( 4 );
				var celula_oc = nova_linha.insertCell( 5 );
				var celula_valor = nova_linha.insertCell( 6 );
				    celula_valor.style.textAlign = 'right';
				    celula_valor.style.color = '#0066cc';
				   
				var celula_valor_exp = nova_linha.insertCell( 7 );
				    celula_valor_exp.style.textAlign = 'right';
				    celula_valor_exp.style.color = '#0066cc';
				var celula_valor_total = nova_linha.insertCell(8);
				    celula_valor_total.style.textAlign = 'right';
				    celula_valor_total.style.color = '#0066cc';
				var celula_acao_alterar = nova_linha.insertCell(9);
				    celula_acao_alterar.style.textAlign = 'center';
				var celula_acao_remover = nova_linha.insertCell(10);
				    celula_acao_remover.style.textAlign = 'center';
				// FIM cria as célular da nova linha
				// cria campos ocultos com os dados do novo item
				var input_hidden_ug = item_financeiro_criar_input( ug, 'ug' );
				var input_hidden_subacao = item_financeiro_criar_input( subacao, 'subacao' );
				var input_hidden_natureza = item_financeiro_criar_input( natureza, 'natureza' );
				var input_hidden_uso = item_financeiro_criar_input( uso, 'uso' );
				var input_hidden_fonte = item_financeiro_criar_input( fonte, 'fonte' );
				var input_hidden_oc = item_financeiro_criar_input( oc, 'oc' );
				var input_hidden_valor = item_financeiro_criar_input( valor, 'valor' );
					input_hidden_valor.id = id + '_valor';
				var input_hidden_valor_exp = item_financeiro_criar_input( valor_exp, 'valor_exp' );
					input_hidden_valor_exp.id = id + '_valor_exp';
                                var input_hidden_total = item_financeiro_criar_input(parseInt(valor.replace(/\./g, ''))
                                    + parseInt(valor_exp.replace(/\./g, '')), 'valor_total');
                                input_hidden_total.id = id + '_valor_total';
			}
			// FIM cria campos ocultos com os dados do novo item
			// define dado para as células de ação
			if ( !permissao_momento || ( item_financeiro_habilita_gnd_1 == false && natureza.substr( 1, 1 ) == '1' ) )
			{
				link_acao_alterar = document.createElement( 'font' );
				link_acao_alterar.innerHTML = ' - ';
				link_acao_remover = document.createElement( 'font' );
				link_acao_remover.innerHTML = ' - ';
			}
			else
			{
				var link_acao_alterar = document.createElement( 'a' );
				    link_acao_alterar.href = 'javascript:item_financeiro_alterar( \'' + id + '\', \'' + ug + '\',  \'' + subacao + '\', \''+ planoorcamentario + '\', \'' + natureza + '\', \'' + uso + '\', \'' + fonte + '\', \'' + oc + '\', \'' + valor + '\', \'' + valor_exp + '\' );';
				    link_acao_alterar.title = 'Editar';
				    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
				var link_acao_remover = document.createElement( 'a' );
				    link_acao_remover.href = 'javascript:item_financeiro_remover( \'' + id + '\', true, \''+ planoorcamentario + '\' );';
				    link_acao_remover.title = 'Excluir';
				    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';
			}
			link_acao_alterar.id = id + '_ancora';
			// FIM define dado para as células de ação
			// insere os dados ( textos e inputs ) nas células
			if( document.getElementById( 'boPlocodigo' ).value == 'true' ){
				celula_ug.appendChild( document.createTextNode( ug ) );
				celula_ug.appendChild( input_hidden_ug );
				celula_subacao.appendChild( document.createTextNode( subacao ) );
				celula_subacao.appendChild( input_hidden_subacao );
				celula_planoorcamentario.appendChild( document.createTextNode( planoorcamentario ) );
				celula_planoorcamentario.appendChild( input_hidden_planoorcamentario );
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
				celula_valor_exp.appendChild( document.createTextNode( valor_exp ) );
				celula_valor_exp.appendChild( input_hidden_valor_exp );
                                celula_valor_total.appendChild(
                                    document.createTextNode(
                                        formata_numero(
                                            parseInt(valor.replace(/\./g, ''))
                                            + parseInt(valor_exp.replace(/\./g, ''))
                                        )
                                    )
                                );
                                celula_valor_total.appendChild(input_hidden_total);
			} else {
				celula_ug.appendChild( document.createTextNode( ug ) );
				celula_ug.appendChild( input_hidden_ug );
				celula_subacao.appendChild( document.createTextNode( subacao ) );
				celula_subacao.appendChild( input_hidden_subacao );
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
				celula_valor_exp.appendChild( document.createTextNode( valor_exp ) );
				celula_valor_exp.appendChild( input_hidden_valor_exp );
                                celula_valor_total.appendChild(
                                    document.createTextNode(
                                        formata_numero(
                                            parseInt(valor.replace(/\./g, ''))
                                            + parseInt(valor_exp.replace(/\./g, ''))
                                        )
                                    )
                                );
                                celula_valor_total.appendChild(input_hidden_total);
			}
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
			item_financeiro_valor_total += (parseInt(desformata_decimal(valor)) + parseInt(desformata_decimal(valor_exp)));
			item_financeiro_atualizar_valor_total();
			// FIM altera soma dos valores de acordo com o novo item
			// atualiza as cores alternadas das linhas
			item_financeiro_organizar_cor_linha();
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
			var ug = document.getElementById( 'nova_ug' );
			var subacao = document.getElementById( 'nova_subacao' );
			var planoorcamentario = document.getElementById( 'nova_planoorcamentario' );
			var natureza = document.getElementById( 'nova_natureza' );
			var uso = document.getElementById( 'novo_uso' );
			var fonte = document.getElementById( 'nova_fonte' );
			var oc = document.getElementById( 'novo_oc' );
			var valor = document.getElementById( 'novo_valor' );
			var valor_exp = document.getElementById( 'novo_valor_expansao' );
                        if (isNaN(parseInt(valor_exp.value))) {
                            valor_exp.value = 0;
                        }

			// realiza validação dos dados
			if ( !validaBranco( ug, 'Unidade Gestora' ) ) return;
			if ( !validaBranco( subacao, 'Subação' ) ) return;
			if ( document.getElementById( 'boPlocodigo' ).value == 'true' ){
				if( !validaBranco( planoorcamentario, 'Plano Orcamentário' ) )return;
			}
			if ( !validaBranco( natureza, 'Natureza da Despesa' ) ) return;
			if ( !validaBranco( uso, 'Ident. de Uso' ) ) return;
			if ( !validaBranco( fonte, 'Fontes de Recursos' ) ) return;
			if ( !validaBranco( oc, 'Ident. Op. de Crédito' ) ) return;
			if ( !validaBranco( valor, 'Valor da Proposta' ) ) return;
			if ( !financeiro_ug_validas[ug.value] )
			{
				alert( 'Unidade Gestora inválida.' );
				ug.focus();
				ug.select();
				return;
			}
			if ( !financeiro_subacao_validas[subacao.value] )
			{
				alert( 'Subação inválida.' );
				subacao.focus();
				subacao.select();
				return;
			}
			
			if( document.getElementById( 'boPlocodigo' ).value == 'true' ){
				if ( planoorcamentario.value != '' )
				{
					if ( !financeiro_planoorcamentario_validas[planoorcamentario.value] )
					{
						alert( 'Plano Orçamentário.' );
						planoorcamentario.focus();
						planoorcamentario.select();
						return;
					}
				}
			}
			
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
			if ( item_financeiro_adicionar( ug.value, subacao.value, planoorcamentario.value, natureza.value, uso.value, fonte.value, oc.value, valor.value, valor_exp.value ) == false )
			{
				return;
			}
			// limpa campos para nova inserção
			ug.value = '';
			subacao.value = '';
			
			if(planoorcamentario.readOnly == false)
				if( document.getElementById( 'boPlocodigo' ).value == 'true' ) planoorcamentario.value = '';
			
			natureza.value = '';
			uso.value = '';
			fonte.value = '';
			oc.value = '';
			valor.value = '';
			valor_exp.value = '';

                        // -- Atualizando o saldo da unidade
                        var saldo = parseInt(
                            document.getElementById('saldo_disponivel').textContent.replace(/\./g, '')
                        );
                        var gasto = parseInt(
                            document.getElementById('novo_valor_total').value.replace(/\./g, '')
                        );
                        document.getElementById('saldo_disponivel').textContent = formata_numero(saldo - gasto);
                        document.getElementById('novo_valor_total').value = '';

			<? if($ugs_existente[0] && count($ugs_existente) == 1): ?>
			ug.value = '<?=$ugs_existente[0]['codigo']; ?>';
			<? endif; ?>

			<? if($subs_existente[0] && count($subs_existente) == 1): ?>
			subacao.value = '<?=$subs_existente[0]['codigo']; ?>';
			<? endif; ?>
			
			<? if($plo_existente[0] && count($plo_existente) == 1): ?>
				planoorcamentario.value = '<?=$plo_existente[0]['codigo']; ?>';
			<? endif; ?>
			item_financeiro_atualizar_fisico();
			item_financeiro_preencher_valores_default();
			houve_modificacao = true;
			document.getElementById( 'nova_subacao' ).focus();
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
		function item_financeiro_alterar( id_tr, ug, subacao, planoorcamentario, natureza, uso, fonte, oc, valor, valor_exp )
		{
			houve_modificacao = true;
			// captura os campos do financeiro
			var campo_ug = document.getElementById( 'nova_ug' );
			var campo_subacao = document.getElementById( 'nova_subacao' );
			if( document.getElementById( 'boPlocodigo' ).value == 'true' ) var campo_planoorcamentario = document.getElementById( 'nova_planoorcamentario' );
			var campo_natureza = document.getElementById( 'nova_natureza' );
			var campo_uso = document.getElementById( 'novo_uso' );
			var campo_fonte = document.getElementById( 'nova_fonte' );
			var campo_oc = document.getElementById( 'novo_oc' );
			var campo_valor = document.getElementById( 'novo_valor' );
			var campo_valor_exp = document.getElementById( 'novo_valor_expansao' );
                        var campo_valor_total = document.getElementById('novo_valor_total');
			campo_ug.value = ug;
			campo_subacao.value = subacao;
			if( document.getElementById( 'boPlocodigo' ).value == 'true' ) campo_planoorcamentario.value = planoorcamentario;
			campo_natureza.value = natureza;
			campo_uso.value = uso;
			campo_fonte.value = fonte;
			campo_oc.value = oc;
			campo_valor.value = valor;
			campo_valor_exp.value = valor_exp;
                        campo_valor_total.value = formata_numero(parseInt(valor.replace(/\./g, '')) + parseInt(valor_exp.replace(/\./g, '')));
			item_financeiro_remover( id_tr, false, planoorcamentario );
			// retira item da lista dos já inseridos
			var id_novo_item = ug + subacao + planoorcamentario + natureza + uso + fonte + oc;
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
				case 'ug':
					var campo = document.getElementById( 'nova_subacao' );
					break;
				case 'subacao':
					var campo = document.getElementById( 'nova_natureza' );
					break;
				case 'planoorcamentario':
					var campo = document.getElementById( 'nova_planoorcamentario' );
					break;
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
		 * @param void
		 * @return void
		 */
		function item_financeiro_atualizar_valor_total()
		{
			var valor_total_string = item_financeiro_valor_total + '';
			valor_total_string = MascaraMonetario( valor_total_string );
			valor_total_string = valor_total_string.substr( 0, valor_total_string.length - 3 );
			//document.getElementById( 'item_financeiro_valor_total' ).innerHTML = valor_total_string;
			document.getElementById( 'item_financeiro_valor_total' ).value = valor_total_string;
		}
		
		/**
		 * Atualiza o valor físico de acordo com o financeiro
		 * 
		 * @return void
		 */
		function item_financeiro_atualizar_fisico()
		{
			var quantidade = document.getElementById( 'fisico_quantidade' );
			if ( quantidade.value == '' || quantidade.value==0 )
			{
				document.getElementById( 'fisico_quantidade' ).value = 1;
			}
			document.getElementById( 'fisico_total' ).value = document.getElementById( 'item_financeiro_valor_total' ).value;
			altera_fisico_total();
		}
		
		/**
		 * Organiza as cores da linhas, ora com valor vazio ora com 'efefef'.
		 * 
		 * @return void
		 */
		function item_financeiro_organizar_cor_linha()
		{
			var tabela = document.getElementById( 'tabela_financeiro' );
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

			if(nome_campo == 'planoorcamentario'){				
				document.getElementById('tr_po_'+valor).style.display = 'table-row';
			}
			
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
		 * @param boolean
		 * @return void
		 */
		function item_financeiro_remover( id_tr, realizar_pergunta, id )
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
			var input = document.getElementById( id_tr + '_valor');
                        var inputExpansao = document.getElementById(id_tr + '_valor_exp');
                        // -- Atualizando o saldo da unidade - removendo o dinheiro ao editar ou excluir um item
                        var saldo = parseInt(
                            document.getElementById('saldo_disponivel').textContent.replace(/\./g, '')
                        );
                        var gasto = (parseInt(desformata_decimal(inputExpansao.value)) + parseInt(desformata_decimal(input.value)));
			if ( tr && input )
			{
                                item_financeiro_valor_total -= 
                                    (parseInt(desformata_decimal(inputExpansao.value)) + parseInt(desformata_decimal(input.value)));
				item_financeiro_atualizar_valor_total();
				tr.parentNode.removeChild( tr );
				item_financeiro_organizar_cor_linha();
			}
			item_financeiro_atualizar_fisico();

                        var novoSaldo = saldo + gasto;
                        document.getElementById('saldo_disponivel').textContent = formata_numero(saldo + gasto);
                        //document.getElementById('novo_valor_total').value = '';

			if(id > 0)
				document.getElementById('tr_po_'+id).style.display = 'none';
		}
		
		// insere itens do financeiro existentes anteriores à exibição do formulário
		document.getElementById( 'item_financeiro_valor_total' ).value = '0';
		<? foreach ( $itens_financeiro as $item_financeiro ) : ?>
			item_financeiro_adicionar( '<?= $item_financeiro['ug'] ?>', '<?= $item_financeiro['subacao'] ?>', '<?=$item_financeiro['planoorcamentario'] ?>',  '<?= $item_financeiro['natureza'] ?>', '<?= $item_financeiro['uso'] ?>', '<?= $item_financeiro['fonte'] ?>', '<?= $item_financeiro['oc'] ?>', '<?= $item_financeiro['valor'] ?>', '<?= $item_financeiro['valor_exp'] ?>' );
		<? endforeach; ?>
		// preenche campos do formulario de item finacneiro com valores padrões
		item_financeiro_preencher_valores_default();
		document.getElementById( 'nova_subacao' ).focus();
		
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
	
	function getElementsByName_iefix(tag, name) 
	{             
	var elem = document.getElementsByTagName(tag);  
	var arr = new Array();  
		for(i = 0,iarr = 0; i < elem.length; i++) {  
			att = elem[i].getAttribute("name");  
			if(att == name) 
			{  
				arr[iarr] = elem[i];  
				iarr++;  
			}  
		}  
		return arr;  
	}
	
	/**
	 * Verifica se formulário este correto e o submete.
	 * 
	 * @return void
	 */
	function validar_cadastro()
	{
		var fisico_valor_total = parseInt( document.getElementById( 'fisico_total' ).value.replace( /\./g, '' ) );
		
//		if ( !possui_item_financeiro_inicial && fisico_valor_total == 0 )
//		{
//			if ( fisico_valor_total <= 0 )
//			{
//				document.getElementById( 'fisico_total' ).focus();
//				alert( 'Dados inválidos. Valor físico deve ser maior que zero.' );
//				return;
//			}
//		}
//		if ( !possui_item_financeiro_inicial && item_financeiro_valor_total == 0 )
//		{
//			if ( item_financeiro_valor_total <= 0 )
//			{
//				alert( 'Dados inválidos. Valor financeiro deve ser maior que zero.' );
//				return;
//			}
//		}
		if ( item_financeiro_valor_total != fisico_valor_total )
		{
//			alert( 'Dados inválidos. Total financeiro e custo total indicado pelo físico são diferentes.' );
//			return;
		}
		
		var formulario = document.formulario;
		var campoPo = getElementsByName_iefix('input', 'planoorcamentario[]');
		var objPlanoOrcamentario = '';

		for(x=0;x<campoPo.length;x++){
			
			objPlanoOrcamentario = document.getElementById('mpovalor_'+campoPo[x].value);
			
			if(objPlanoOrcamentario){
				if(objPlanoOrcamentario.value == '' && objPlanoOrcamentario.alt){
					alert('A quantidade do Plano Orçamentário '+campoPo[x].value+' é obrigatória!');
					objPlanoOrcamentario.focus();
					return false;
				}
			}
		}

		/*if( document.getElementById( 'boMetas' ).value == 'true' ){
			for(i=0; i<formulario.length; i++){
				if(formulario.elements[i].id.indexOf('mpovalor') != -1 ){
					if( formulario.elements[i].value == '' ){
						alert('A quantidade do Plano Orçamentário é obrigatória!');
						formulario.elements[i].focus();
						return false;
					}
				}
			}
		}*/

		// valida campos obrigatórios
//                if (!validaBranco(formulario.expansao_fisico_quantidade, 'Limite Adicional')) return;
		if (!validaBranco(formulario.justificativa, 'Justificativa')) return;
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
	<? endif; ?>
	
	<? if ( $ppoid ) : ?>
		bloquear_escolha_proposta();
	<? endif; ?>
	
	<? if ( $acaid && $ppoid ) : ?>
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
