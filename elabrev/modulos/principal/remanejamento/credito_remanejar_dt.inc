<?php
/*
  Sistema Simec
  Setor responsável: SPO-MEC
  Desenvolvedor: Equipe Consultores Simec
  Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
  Programador: Renan de Lima Barbosa (renandelima@gmail.com)
  Módulo: credito_remanejar.inc
  Finalidade: remanejar créditos
 */

######################################
#                                    #
#       ABREVIAÇÕES UTILIZADAS       #
#	                                 #
#  dot  dotação inicial              #
#  sup  suplementação                #
#  can  cancelamento                 #
#  out  outros(as)                   #
#                                    #
######################################
// variáveis de origem do credito.inc
/*
  $tcrid;
  $unicod;
  $prgcod;
  $acacod;
  $loccod;
 */

// Regra para atender a demanda #224117
if ($_SESSION['credito_mcrid'] === 45) {
    if ($_POST['tcrid'] != '209' && $_POST['tcrid'] != '217' && $_POST['tcrid'] != '212') {
        die("<script>
                    alert('Para o Momento Adicional de crédito 2013 serão aceitos apenas os créditos 101, 112 e 157.'); 
                    window.location.href = '/elabrev/elabrev.php?modulo=principal/remanejamento/credito&acao=C';
                    //window.history.go(-1);
             </script>");
    }
}

require_once 'tipocredito_funcoes.inc';

$edicao_ativa = cre_edicao_ativa();

$_SESSION['creditos_adicionais_tcrid'] = $tcrid; // -- definido em credito.inc
$_SESSION['creditos_adicionais_unicod'] = $unicod; // -- definido em credito.inc

if ($_REQUEST['ungcod']) {
    $ungcod = $_REQUEST['ungcod'];
} elseif ($_REQUEST['ungcod_creditos']) {
    $ungcod = $_REQUEST['ungcod_creditos'];
} else {
    $ungcod = null;
}

// exibe cabecalho primeiro para mostrar animação de "carregando"
include APPRAIZ . 'includes/cabecalho.inc';

//captura dados do tipo de crédito
$tipocredito = tc_pegar($tcrid);
$tipocredito['tcrnaturezapropria'] = ($tipocredito['tcrnaturezapropria'] == 't');

// ---------- PROCESSAMENTO DOS DADOS DO FORMULÁRIO

$creditos_extra = array();
$erro_credito_extra = array();

if ($edicao_ativa && isset($_REQUEST['remanejar'])) {
    // verifica créditos extra

    for ($i = 0; $i < count($_REQUEST['extra']['prgcod']); $i++) {
        $erro_item_extra = array();

        // tenta pegar acaid
        $extra_prgcod = $_REQUEST['extra']['prgcod'][$i];
        $extra_acacod = $_REQUEST['extra']['acacod'][$i];
        $extra_loccod = $_REQUEST['extra']['loccod'][$i];
        $extra_irpcod = $_REQUEST['extra']['irpcod'][$i];

        if ($_REQUEST['extra']['plocod'][$i] != '') {
            $extra_plocod = "'" . $_REQUEST['extra']['plocod'][$i] . "'";
        } else {
            $extra_plocod = "'0000'";
        }

        // verifica se existe ação
        $acaid = cre_pegar_acaid($unicod, $extra_prgcod, $extra_acacod, $extra_loccod);
        if (!$acaid) {
            $verificacao_acao = false;
            if ($tipocredito['tcrcod'] == '200' && strlen($extra_prgcod) == 4 && strlen($extra_acacod) == 4 && strlen($extra_loccod) == 4) {
                $sql = "
					select
						acadsc
					from monitora.acao
					where
						acastatus = 'A' and
						acacod = '" . $extra_acacod . "' and
						prgano = '" . $_SESSION['exercicio'] . "'
					order by acasnrap";
                $dados_acao = $db->recuperar($sql);
                $sql = "
					select
						prgid
					from monitora.programa
					where
						prgstatus = 'A' and
						prgcod = '" . $extra_prgcod . "' and
						prgano = '" . $_SESSION['exercicio'] . "'";
                $dados_programa = $db->recuperar($sql);
                if ($dados_programa) {
                    $sql = "select nextval( 'monitora.acao_acaid_seq' ) as acaid";
                    $acaid = (integer) $db->pegaUm($sql);
                    $sql = sprintf(
                        " insert into monitora.acao " .
                        " ( acaid, unicod, unitpocod, prgcod, acacod, loccod, saccod, prgano, acastatus, acasnrap, acadsc, prgid ) " .
                        " values " .
                        " ( %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %s, '%s', %s ) ", $acaid, $unicod, 'U', $extra_prgcod, $extra_acacod, $extra_loccod, '', $_SESSION['exercicio'], 'I', 'false', $dados_acao['acadsc'], $dados_programa['prgid']
                    );
                    $db->executar($sql);
                    $verificacao_acao = true;
                }
            }
            if ($verificacao_acao == false) {
                array_push(
                    $erro_credito_extra, 'Ação ' .
                    ' com Programa ' . $extra_prgcod .
                    ', código ' . $extra_acacod .
                    ' e Localizador ' . $extra_loccod .
                    ' para a Unidade ' . $unicod .
                    ' não existe.'
                );
                array_push($erro_item_extra, 'prg');
                array_push($erro_item_extra, 'aca');
                array_push($erro_item_extra, 'loc');
            }
        }

        // natureza de receita ou de despesa
        $extra_ndpcod = $_REQUEST['extra']['ndpcod'][$i];
        //$extra_nrccod = $_REQUEST['extra']['ndpcod'][$i];
        $existe_ndp = cre_ndpcod_existe($extra_ndpcod);
        //$existe_nrc = false;
        /*
          if ( $tipocredito['tcrnaturezapropria'] )
          {
          $existe_nrc = cre_nrccod_existe( $extra_nrccod );
          }
         */
        if (!$existe_ndp) { //&& !$existe_nrc  )
            array_push($erro_credito_extra, 'Não existe Natureza de código ' . $extra_ndpcod);
            array_push($erro_item_extra, 'ndp');
        }
        //if ( $existe_ndp )
        else {
            $sql = "select ndpid from public.naturezadespesa where ndpcod = '" . $extra_ndpcod . "'";
            $extra_ndpid = $db->pegaUm($sql);
            $extra_nrcid = 'null';
        }
        /*
          else
          {
          $sql = "select nrcid from public.naturezareceita where nrccod = '" . $extra_nrccod . "'";
          $extra_ndpid = 'null';
          $extra_nrcid = $db->pegaUm( $sql );
          }
         */

        $extra_foncod = $_REQUEST['extra']['foncod'][$i];
        if (!cre_foncod_existe($extra_foncod)) {
            array_push($erro_credito_extra, 'Fonte ' . $extra_ndpcod . ' não existe.');
            array_push($erro_item_extra, 'fon');
        }

        $extra_idocod = $_REQUEST['extra']['idocod'][$i];
        if (!cre_idocod_existe($extra_idocod)) {
            array_push($erro_credito_extra, 'IDOC ' . $extra_idocod . ' não existe.');
            array_push($erro_item_extra, 'ido');
        }
        $sql = "select idoid from public.idoc where idocod = '" . $extra_idocod . "'";
        $extra_idoid = $db->pegaUm($sql);

        $extra_iducod = $_REQUEST['extra']['iducod'][$i];
        if (!cre_iducod_existe($extra_iducod)) {
            array_push($erro_credito_extra, 'IDUSO ' . $extra_iducod . ' não existe.');
            array_push($erro_item_extra, 'idu');
        }

        $extra = array(
            'prgcod' => $extra_prgcod,
            'acacod' => $extra_acacod,
            'loccod' => $extra_loccod,
            'plocod' => $extra_plocod,
            'acaidloa' => $acaid,
            'ndpcod' => $extra_ndpcod,
            'ndpid' => $extra_ndpid,
            //'nrccod'   => $extra_nrccod,
            'nrcid' => $extra_nrcid,
            'foncod' => $extra_foncod,
            'idoid' => $extra_idoid,
            'idocod' => $extra_idocod,
            'iducod' => $extra_iducod,
            'irpcod' => $extra_irpcod,
            'sup' => cre_pegar_numero_cru($_REQUEST['extra']['sup'][$i]),
            'can' => cre_pegar_numero_cru($_REQUEST['extra']['can'][$i]),
            'vlexcesso' => cre_pegar_numero_cru($_REQUEST['extra']['vlexcesso'][$i]),
            'vlsuperavit' => cre_pegar_numero_cru($_REQUEST['extra']['vlsuperavit'][$i]),
            'erro' => $erro_item_extra
        );
        array_push($creditos_extra, $extra);
    }

    if (count($erro_credito_extra) == 0) {

        // remove os itens de despesa que estão sendo trabalhados
        /*
          $sucesso = cre_remover( $tcrid, array( $unicod ), array( $prgcod ), array( $acacod ), array( $loccod ) );
          if ( !$sucesso )
          {
          dbg( 'ERRO!' );
          dbg( 'Falha ao remover os itens de despesa que estão sendo trabalhados', 1 );
          }
         */

        //$sqlUngcod = ($ungcod) ? " AND ungcod = '" . $ungcod . "' " : "";
        //$sqlSetUngcod = ($ungcod) ? " , ungcod = '" . $ungcod . "' " : "";

        $sql_base_despesa_atualizar = "update
					 elabrev.despesaacao
				set
					 dpavalor = %d
		{$sqlSetUngcod}
				where
					 plocod   = %s and
					 iducod   = '%s' and
					 foncod   = '%s' and
					 ndpid       %s  and
					 nrcid       %s  and
					 acaidloa =  %d  and
					 idoid    =  %d  and
					 ppoid    =  %d  and
					 tcrid    =  %d  and
					 mcrid    = {$_SESSION['credito_mcrid']}
					 
					";

        $sql_base_despesa_atualizar_can = "$sql_base_despesa_atualizar
			 and dpavalor < 0";

        $sql_base_despesa_atualizar_sup = "$sql_base_despesa_atualizar
			 and dpavalor > 0";

        $sql_base_despesa_atualizar_excesso = "$sql_base_despesa_atualizar
			 and dpavalor > 0
			 and dpatipovalor = 3";

        $sql_base_despesa_atualizar_superavit = "$sql_base_despesa_atualizar
			 and dpavalor > 0
			 and dpatipovalor = 4";

        $sql_base_despesa_existe = "select
					 count(*)
				from
					 elabrev.despesaacao
				where
					 iducod   = '%s' and
					 plocod	  = %s and
					 foncod   = '%s' and
					 ndpid       %s  and
					 nrcid       %s  and
					 acaidloa =  %d  and
					 idoid    =  %d  and
					 ppoid    =  %d  and
					 mcrid    = {$_SESSION['credito_mcrid']}
					";

        $sql_base_despesa_existe_dot = "select
					 count(*)
				from
					 elabrev.despesaacao
				where
					 iducod   = '%s' and
					 plocod	  = %s and
					 foncod   = '%s' and
					 ndpid       %s  and
					 nrcid       %s  and
					 acaidloa =  %d  and
					 idoid    =  %d  and
					 ppoid    =  %d  and
					 mcrid    is null and
					 tcrid    is null
					 ";

        $sql_base_despesa_existe_can = "$sql_base_despesa_existe
			and tcrid = %d
			and dpavalor < 0
			and (dpatipovalor = 1 or dpatipovalor is null)";

        $sql_base_despesa_existe_sup = "$sql_base_despesa_existe
			and tcrid = %d
			and dpavalor > 0
			and (dpatipovalor = 2 or dpatipovalor is null)";

        $sql_base_despesa_existe_excesso = "$sql_base_despesa_existe
			and tcrid = %d
			and dpavalor > 0
			and dpatipovalor = 3";

        $sql_base_despesa_existe_superavit = "$sql_base_despesa_existe
			and tcrid = %d
			and dpavalor > 0
			and dpatipovalor = 4";


        if ($ungcod) {
            //$campoUngcod = ",ungcod";
            //$valorUngcod = ",'".$ungcod."'";
        } else {
            //$campoUngcod = "";
            //$valorUngcod = "";
        }

        $sql_base_despesa_inserir = "insert into elabrev.despesaacao
				(
				plocod,
					 dpavalor,
					 dpatipovalor,
					 iducod,
					 foncod,
					 ndpid,
					 nrcid,
					 acaidloa,
					 idoid,
					 ppoid,
					 tcrid
,					 mcrid
					 {$campoUngcod}
				)
				values
				(
					%s,
					 %d,
					 %d,
					 '%s',
					 '%s',
					 %s,
					 %s,
					 %d,
					 %d,
					 %d,
					 %s,
					 {$_SESSION['credito_mcrid']}
					{$valorUngcod}
				)";

        $sql_base_despesa_inserir_dotacao = "insert into elabrev.despesaacao
				(
				plocod,
				irpcod,
					 dpavalor,
					 dpatipovalor,
					 iducod,
					 foncod,
					 ndpid,
					 nrcid,
					 acaidloa,
					 idoid,
					 ppoid,
					 tcrid
					 {$campoUngcod}
				)
				values
				(
					%s,
					%d,
					 %d,
					 %d,
					 '%s',
					 '%s',
					 %s,
					 %s,
					 %d,
					 %d,
					 %d,
					 %s
					 {$valorUngcod}
				)";

        // cria novos itens para os créditos extra
        $sucesso = true;
        $ppoid = cre_pegar_proposta_ativa();
        foreach ($creditos_extra as $extra) {
            // verifica se existe dotação inicial
            $sql = sprintf(
                $sql_base_despesa_existe_dot, $extra['iducod'], $extra['plocod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? ' = ' . $extra['ndpid'] : ' is null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? ' = ' . $extra['nrcid'] : ' is null ', $extra['acaidloa'], $extra['idoid'], $ppoid
            );
            $dot_existe = $db->pegaUm($sql);
            if (!$dot_existe) {
                // se despesa não existe cria com dotação inicial cam valor ZERO
                $sql = sprintf(
                    $sql_base_despesa_inserir_dotacao, $extra['plocod'], $extra['irpcod'], 0, 0, $extra['iducod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? $extra['ndpid'] : ' null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? $extra['nrcid'] : ' null ', $extra['acaidloa'], $extra['idoid'], $ppoid, 'null'
                );
                if (!$db->executar($sql)) {
                    $sucesso = false;
                    break;
                }
            }

            // verifica se existe cancelamento can
            $sql = sprintf(
                $sql_base_despesa_existe_can, $extra['iducod'], $extra['plocod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? ' = ' . $extra['ndpid'] : ' is null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? ' = ' . $extra['nrcid'] : ' is null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
            );


            $can_existe = $db->pegaUm($sql);

            if (!$can_existe) { //inserir
                // se despesa não existe cria cancelamento
                $sql = sprintf(
                    $sql_base_despesa_inserir, $extra['plocod'], ($extra['can'] * -1), 1, $extra['iducod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? $extra['ndpid'] : ' null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? $extra['nrcid'] : ' null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
                );
            } else { //alterar
                // se existe atualiza cancelamento
                $sql = sprintf(
                    $sql_base_despesa_atualizar_can, ($extra['can'] * -1), $extra['plocod'], $extra['iducod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? ' = ' . $extra['ndpid'] : ' is null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? ' = ' . $extra['nrcid'] : ' is null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
                );
            }
            if (!$db->executar($sql)) {
                $sucesso = false;
                break;
            }
            // fim can
            // verifica se existe suplementação no credito extra
            $sql = sprintf(
                $sql_base_despesa_existe_sup, $extra['iducod'], $extra['plocod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? ' = ' . $extra['ndpid'] : ' is null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? ' = ' . $extra['nrcid'] : ' is null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
            );
            $sup_existe = $db->pegaUm($sql);

            if (!$sup_existe) { //inserir
                // se despesa não existe cria suplementação
                $sql = sprintf(
                    $sql_base_despesa_inserir, $extra['plocod'], $extra['sup'], 2, $extra['iducod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? $extra['ndpid'] : ' null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? $extra['nrcid'] : ' null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
                );
            } else { //alterar
                // se existe atualiza suplementação
                $sql = sprintf(
                    $sql_base_despesa_atualizar_sup, $extra['sup'], $extra['plocod'], $extra['iducod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? ' = ' . $extra['ndpid'] : ' is null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? ' = ' . $extra['nrcid'] : ' is null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
                );
            }
            if (!$db->executar($sql)) {
                $sucesso = false;
                break;
            }
            // fim sup
            // verifica se existe suplementação excesso
            $sql = sprintf(
                $sql_base_despesa_existe_excesso, $extra['iducod'], $extra['plocod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? ' = ' . $extra['ndpid'] : ' is null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? ' = ' . $extra['nrcid'] : ' is null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
            );
            $excesso_existe = $db->pegaUm($sql);

            if (!$excesso_existe) { //inserir
                // se despesa não existe cria suplementação
                $sql = sprintf(
                    $sql_base_despesa_inserir, $extra['plocod'], $extra['vlexcesso'], 3, $extra['iducod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? $extra['ndpid'] : ' null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? $extra['nrcid'] : ' null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
                );
            } else { //alterar
                // se existe atualiza suplementação
                $sql = sprintf(
                    $sql_base_despesa_atualizar_excesso, $extra['vlexcesso'], $extra['plocod'], $extra['iducod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? ' = ' . $extra['ndpid'] : ' is null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? ' = ' . $extra['nrcid'] : ' is null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
                );
            }
            if (!$db->executar($sql)) {
                $sucesso = false;
                break;
            }
            // fim excesso
            //verifica se existe suplementação superavit
            $sql = sprintf(
                $sql_base_despesa_existe_superavit, $extra['iducod'], $extra['plocod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? ' = ' . $extra['ndpid'] : ' is null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? ' = ' . $extra['nrcid'] : ' is null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
            );
            $superavit_existe = $db->pegaUm($sql);

            if (!$superavit_existe) { //inserir
                // se despesa não existe cria suplementação
                $sql = sprintf(
                    $sql_base_despesa_inserir, $extra['plocod'], $extra['vlsuperavit'], 4, $extra['iducod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? $extra['ndpid'] : ' null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? $extra['nrcid'] : ' null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
                );
            } else { //alterar
                // se existe atualiza suplementação
                $sql = sprintf(
                    $sql_base_despesa_atualizar_superavit, $extra['vlsuperavit'], $extra['plocod'], $extra['iducod'], $extra['foncod'], $extra['ndpid'] && $extra['ndpid'] != 'null' ? ' = ' . $extra['ndpid'] : ' is null ', $extra['nrcid'] && $extra['nrcid'] != 'null' ? ' = ' . $extra['nrcid'] : ' is null ', $extra['acaidloa'], $extra['idoid'], $ppoid, $tcrid
                );
            }
            if (!$db->executar($sql)) {
                $sucesso = false;
                break;
            }
            // fim excesso
        }
        if (!$sucesso) {
            dbg('ERRO!');
            dbg('Falha ao inserir itens extra de despesa', 1);
        }

        /**
         * valores a serem verificador após leitura das despesas
         * varia de acordo com o campo tcrtipoporcento do tipo de credito
         * o codigo pode representar unidade ou localizador
         *
         * 	cod => array(
         * 		'dot' => 0,
         * 		'can' => 0
         * 		'sup' => 0
         * 	)
         *
         */
        $remanejado = array();

        $dpaids = is_array($_REQUEST['credito_sup']) ? array_keys($_REQUEST['credito_sup']) : array();
        $erro = array(); // erros que são encontrados
        $comandos = array(); // comandos a serem executados após verificação

        foreach ($dpaids as $dpaid) {
            $despesa = cre_pegar_dados_despesa($dpaid);

            $saldo_atual = cre_pegar_saldo_atual($dpaid);

            $can = cre_pegar_numero_cru($_REQUEST['credito_can'][$dpaid]);
            $sup = cre_pegar_numero_cru($_REQUEST['credito_sup'][$dpaid]);
            $vlexcesso = cre_pegar_numero_cru($_REQUEST['credito_vlexcesso'][$dpaid]);
            $vlsuperavit = cre_pegar_numero_cru($_REQUEST['credito_vlsuperavit'][$dpaid]);


            //(string) $despesa['plocod'] = str_pad($despesa['plocod'], 4, "0", STR_PAD_LEFT);

            $despesa['plocod'] = "'" . $despesa['plocod'] . "'";

            // a verificação de porcentagem é realizada em relação à unidade ou
            // localizador, dependendo do tipo de crédito
            // acumula valor suplementado e cancelado de acordo com o tipo de
            // credito para posterior verificação de limite de porcentagem de
            // remanejamento


            $cod = $tipocredito['tcrtipoporcento'] == 'L' ? $despesa['loccod'] : $despesa['unicod'];
            if (!array_key_exists($cod, $remanejado)) {
                $remanejado[$cod] = array(
                    'dot' => cre_pegar_dotacao_inicial($cod, $tipocredito['tcrtipoporcento']),
                    'can' => 0,
                    'sup' => 0,
                    'vlexcessao' => 0,
                    'vlsuperavit' => 0
                );
            }
            $remanejado[$cod]['can'] += $can;
            $remanejado[$cod]['sup'] += $sup;
            $remanejado[$cod]['vlexcessao'] += $vlexcessao;
            $remanejado[$cod]['vlsuperavit'] += $vlsuperavit;

            // TODO: verificar ação
            // TODO: verificar gnd
            // TODO: verificar tipo de unidade
            // TODO: verificar unidade
            // verifica saldo do item de despesa
            /*
              $saldo_futuro = $saldo_atual - $can + $sup;
              if ( $saldo_futuro < 0 )
              {
              array_push( $erro, "Saldo na despesa de identificador " . $dpaid . " insuficiente para realizar operação" );
              continue;
              }
             */

            // ---- APLICA CANCELAMENTO
            // verifica se existe cancelamento can
            $sql = sprintf(
                $sql_base_despesa_existe_can, $despesa['iducod'], $despesa['plocod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' = ' . $despesa['ndpid'] : ' is null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' = ' . $despesa['nrcid'] : ' is null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
            );

            $can_existe = $db->pegaUm($sql);

            $can_sql = '';
            if (!$can_existe) {
                // se não existe suplementação cria sql para inserção
                if ($can > 0) {
                    $can_sql = sprintf(
                        $sql_base_despesa_inserir, $despesa['plocod'], $can * -1, 1, $despesa['iducod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? $despesa['ndpid'] : ' null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? $despesa['nrcid'] : ' null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
                    );
                }
            } else {
                // se existe suplementação cria sql para atualização
                $can_sql = sprintf(
                    $sql_base_despesa_atualizar_can, $can * -1, $despesa['plocod'], $despesa['iducod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' = ' . $despesa['ndpid'] : ' is null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' = ' . $despesa['nrcid'] : ' is null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
                );
            }
            if ($can_sql) {
                array_push($comandos, $can_sql);
            }

//				echo 'CANCELAMENTO';
//				dbg( $comandos);
//				$comandos = array();
            //fim can
            // ---- FIM APLICA CANCELAMENTO
            // ---- APLICA SUPLEMENTAÇÂO
            // verifica se existe suplementação sup
            $sql = sprintf(
                $sql_base_despesa_existe_sup, $despesa['iducod'], $despesa['plocod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' = ' . $despesa['ndpid'] : ' is null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' = ' . $despesa['nrcid'] : ' is null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
            );

            $sup_existe = $db->pegaUm($sql);

            $sup_sql = '';
            if (!$sup_existe) {
                // se não existe suplementação cria sql para inserção
                if ($sup > 0) {
                    $sup_sql = sprintf(
                        $sql_base_despesa_inserir, $despesa['plocod'], $sup, 2, $despesa['iducod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' ' . $despesa['ndpid'] : ' null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' ' . $despesa['nrcid'] : ' null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
                    );
                }
            } else {
                // se existe suplementação cria sql para atualização
                $sup_sql = sprintf(
                    $sql_base_despesa_atualizar_sup, $sup, $despesa['plocod'], $despesa['iducod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' = ' . $despesa['ndpid'] : ' is null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' = ' . $despesa['nrcid'] : ' is null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
                );
            }
            if ($sup_sql) {
                array_push($comandos, $sup_sql);
            }
//				ver( $sup_sql);
//				die;
            //fim sup
//				echo 'SUPLEMENTACAO';
//				dbg( $comandos);
//				$comandos = array();
            // verifica se existe suplementação excesso
            $sql = sprintf(
                $sql_base_despesa_existe_excesso, $despesa['iducod'], $despesa['plocod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' = ' . $despesa['ndpid'] : ' is null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' = ' . $despesa['nrcid'] : ' is null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
            );
            $excesso_existe = $db->pegaUm($sql);
            $excesso_sql = '';
            if (!$excesso_existe) {
                // se não existe suplementação cria sql para inserção
                if ($vlexcesso > 0) {
                    $excesso_sql = sprintf(
                        $sql_base_despesa_inserir, $despesa['plocod'], $vlexcesso, 3, $despesa['iducod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' ' . $despesa['ndpid'] : ' null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' ' . $despesa['nrcid'] : ' null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
                    );
                }
            } else {
                // se existe suplementação cria sql para atualização
                $excesso_sql = sprintf(
                    $sql_base_despesa_atualizar_excesso, $vlexcesso, $despesa['plocod'], $despesa['iducod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' = ' . $despesa['ndpid'] : ' is null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' = ' . $despesa['nrcid'] : ' is null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
                );
            }
            if ($excesso_sql) {
                array_push($comandos, $excesso_sql);
            }

//				echo 'EXCESSO';
//				dbg( $comandos);
//				$comandos = array();
            //fim excesso
            // verifica se existe suplementação superavit
            $sql = sprintf(
                $sql_base_despesa_existe_superavit, $despesa['iducod'], $despesa['plocod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' = ' . $despesa['ndpid'] : ' is null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' = ' . $despesa['nrcid'] : ' is null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
            );
            $superavit_existe = $db->pegaUm($sql);
            $superavit_sql = '';
            if (!$superavit_existe) {
                // se não existe suplementação cria sql para inserção
                if ($vlsuperavit > 0) {
                    $superavit_sql = sprintf(
                        $sql_base_despesa_inserir, $despesa['plocod'], $vlsuperavit, 4, $despesa['iducod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' ' . $despesa['ndpid'] : ' null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' ' . $despesa['nrcid'] : ' null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
                    );
                }
            } else {
                // se existe suplementação cria sql para atualização
                $superavit_sql = sprintf(
                    $sql_base_despesa_atualizar_superavit, $vlsuperavit, $despesa['plocod'], $despesa['iducod'], $despesa['foncod'], $despesa['ndpid'] && $despesa['ndpid'] != 'null' ? ' = ' . $despesa['ndpid'] : ' is null ', $despesa['nrcid'] && $despesa['nrcid'] != 'null' ? ' = ' . $despesa['nrcid'] : ' is null ', $despesa['acaidloa'], $despesa['idoid'], $ppoid, $tcrid
                );
            }
            if ($superavit_sql) {
                array_push($comandos, $superavit_sql);
            }
            //fim excesso
            // ---- FIM APLICA SUPLEMENTAÇÂO
        }

        //dbg($comandos,1);
        // verifica quais erros aconteceram
        if (count($erro) > 0) {
            $db->rollback();
            dbg('ERRO!');
            dbg($erro, 1);
        } else {
            foreach ($comandos as $sql) {
                if (!$db->executar($sql)) {
                    $db->rollback();
                    dbg('ERRO!');
                    dbg('Falha ao inserir os dados de remanejamento', 1);
                }
            }
            $creditos_extra = array();
            $db->commit();
        }
    } else {
        //dbg( $_REQUEST['extra'] );
        //dbg( $creditos_extra );
    }
}

// ---------- FIM PROCESSAMENTO DOS DADOS DO FORMULÁRIO

$tcrdsc = $unidsc = $prgdsc = $acadsc = $locdsc = '';

// -- Processando filtros da listagem de remanejamento -----------------------------------------------------------------

// -- Descrição e código do tipo de crédito
$sql = "select tcrdsc, tcrcod from elabrev.tipocredito where tcrid = {$tcrid}";
$dados = $db->recuperar($sql);
$tcrcod = $dados['tcrcod'];
$tcrdsc = $dados['tcrdsc'];

// -- Descrição da unidade orçamentária selecionada (via link)
$sql = "select unidsc from public.unidade where unicod = '{$unicod}'";
$unidsc = $db->pegaUm($sql);

// -- Descrição do programa, se selecionado no filtro da tela de remanejamento
if ($prgcod) { // -- definido em credito.inc
    $sql = "select prgdsc from monitora.programa where prgcod = '{$prgcod}";
    $prgdsc = $db->pegaUm($sql);
}

// -- Descrição da ação, se selecionada no fitro da tela de remanejamento
if ($acacod) { // -- definido em credito.inc
    $sql = "select acadsc from monitora.acao where acacod = '{$acacod}' group by acadsc";
    $acadsc = $db->pegaUm($sql);
}

// -- Descrição do localizador, se selecionado no filtro da tela de remanejamento
if ($loccod) { // -- definido em credito.inc
    $sql = "select locdsc from public.localizador where loccod = '{$loccod}'";
    $locdsc = $db->pegaUm($sql);
}

// captura proposta que está sendo trabalhada
$ppoid = cre_pegar_proposta_ativa();

$condicao = array();
$condicao_invalido_fixo = array();
array_push($condicao, " a.prgano = '{$_SESSION['exercicio']}' ");
array_push($condicao, " a.acasnrap = 'f' ");
array_push($condicao, " d.ppoid = {$ppoid} ");
array_push($condicao, " d.tcrid IS NULL ");
array_push($condicao_invalido_fixo, " a.prgano = '{$_SESSION['exercicio']}' ");
array_push($condicao_invalido_fixo, " a.acasnrap = 'f' ");
array_push($condicao_invalido_fixo, " d.ppoid = {$ppoid} ");

$unicod2 = array($unicod);
if ($unicod2 && $unicod2[0]) {
    if (count($unicod2) > 1) {
        array_push($condicao, " a.unicod in ( '" . implode("','", $unicod2) . "' ) ");
        array_push($condicao_invalido_fixo, " a.unicod in ( '" . implode("','", $unicod2) . "' ) ");
    } else {
        array_push($condicao, " a.unicod = '" . $unicod2[0] . "' ");
        array_push($condicao_invalido_fixo, " a.unicod = '" . $unicod2[0] . "' ");
    }
}
$prgcod2 = array($prgcod);
if ($prgcod2 && $prgcod2[0]) {
    if (count($prgcod2) > 1) {
        array_push($condicao, " a.prgcod in ( '" . implode("','", $prgcod2) . "' ) ");
        array_push($condicao_invalido_fixo, " a.prgcod in ( '" . implode("','", $prgcod2) . "' ) ");
    } else {
        array_push($condicao, " a.prgcod = '" . $prgcod2[0] . "' ");
        array_push($condicao_invalido_fixo, " a.prgcod = '" . $prgcod2[0] . "' ");
    }
}
$acacod2 = array($acacod);
if ($acacod2 && $acacod2[0]) {
    if (count($acacod2) > 1) {
        array_push($condicao, " a.acacod in ( '" . implode("','", $acacod2) . "' ) ");
        array_push($condicao_invalido_fixo, " a.acacod in ( '" . implode("','", $acacod2) . "' ) ");
    } else {
        array_push($condicao, " a.acacod = '" . $acacod2[0] . "' ");
        array_push($condicao_invalido_fixo, " a.acacod = '" . $acacod2[0] . "' ");
    }
}
$loccod2 = array($loccod);
if ($loccod2 && $loccod2[0]) {
    if (count($loccod2) > 1) {
        array_push($condicao, " a.loccod in ( '" . implode("','", $loccod2) . "' ) ");
        array_push($condicao_invalido_fixo, " a.loccod in ( '" . implode("','", $loccod2) . "' ) ");
    } else {
        array_push($condicao, " a.loccod = '" . $loccod2[0] . "' ");
        array_push($condicao_invalido_fixo, " a.loccod = '" . $loccod2[0] . "' ");
    }
}

// -- Processando filtros de tipo de crédito
// acaid
$sql = "select acaid from elabrev.tipocreditoacao where tcrid = {$tcrid}";
$dados = $db->carregar($sql);
if ($dados) {
    $cods = array();
    foreach ($dados as $linha) {
        array_push($cods, $linha['acaid']);
    }
    array_push($condicao, " d.acaidloa in ( '" . implode("','", $cods) . "' ) ");
}
// gnd
$sql = "select gndcod from elabrev.tipocreditognd where tcrid = {$tcrid}";
$dados = $db->carregar($sql);
if ($dados) {
    $cods = array();
    foreach ($dados as $linha) {
        array_push($cods, $linha['gndcod']);
    }
    array_push($condicao, " n.gndcod in ( '" . implode("','", $cods) . "' ) ");
}
// grupo unidade
$sql = "select gunid from elabrev.tipocreditogrupounidade where tcrid = {$tcrid}";
$dados = $db->carregar($sql);
if ($dados) {
    $cods = array();
    foreach ($dados as $linha) {
        array_push($cods, $linha['gunid']);
    }
    array_push($condicao, " u.gunid in ( '" . implode("','", $cods) . "' ) ");
}
// unidade
$sql = "select unicod, unitpocod from elabrev.tipocreditounidade where tcrid = {$tcrid}";
$dados = $db->carregar($sql);
if ($dados) {
    $cods_cod = array();
    $cods_tpo = array();
    foreach ($dados as $linha) {
        array_push($cods_cod, $linha['unicod']);
        array_push($cods_tpo, $linha['unitpocod']);
    }
    array_push($condicao, " u.unicod in ( '" . implode("','", $cods_cod) . "' ) ");
    array_push($condicao, " u.unitpocod in ( '" . implode("','", $cods_tpo) . "' ) ");
}
$where = implode(" and ", $condicao);

// captura os créditos que NÃO pertencem ao filtro (dados cadastrados erroneamente pelo usuário)
$where_invalidos = array();
$condicao_invalido_fixo = implode(' and ', $condicao_invalido_fixo);
$ano = $_SESSION['exercicio'];
$sql = <<<EOT
SELECT d.*
       -- d.acaidloa,
       -- d.ndpid,
       -- d.foncod,
       -- d.idoid,
       -- d.iducod
  FROM elabrev.despesaacao d
    INNER JOIN monitora.acao a ON a.acaid = d.acaidloa
    INNER JOIN public.unidade u ON u.unicod = a.unicod
    INNER JOIN public.naturezadespesa n ON n.ndpid = d.ndpid
    INNER JOIN public.fonterecurso f ON f.foncod = d.foncod
    INNER JOIN elabrev.tipocredito t ON t.tcrid = d.tcrid
  WHERE d.dpavalor != 0
    AND d.tcrid = $tcrid
    AND {$condicao_invalido_fixo}
    AND (d.acaidloa NOT IN(SELECT acaid
                             FROM elabrev.tipocreditoacao
                             WHERE tcrid = {$tcrid})
         OR n.gndcod NOT IN(SELECT gndcod
                              FROM elabrev.tipocreditognd
                              WHERE tcrid = {$tcrid})
         OR u.gunid NOT IN(SELECT gunid
                             FROM elabrev.tipocreditogrupounidade
                             WHERE tcrid = {$tcrid})
         OR a.unicod NOT IN(SELECT unicod
                              FROM elabrev.tipocreditounidade
                              WHERE tcrid = {$tcrid}))
    AND d.mcrid = {$_SESSION['credito_mcrid']}
EOT;
$invalidos = $db->carregar($sql);
//dbg( $sql, 1 );
$invalidos = $invalidos ? $invalidos : array();
foreach ($invalidos as $item) {
    array_push(
        $where_invalidos, " ( " .
        " $condicao_invalido_fixo and " .
        " d.acaidloa = " . $item['acaidloa'] . " and " .
        " d.ndpid = " . $item['ndpid'] . " and " .
        " d.foncod = '" . $item['foncod'] . "' and " .
        " d.idoid = " . $item['idoid'] . " and " .
        " d.iducod = '" . $item['iducod'] . "' and " .
        //
        " d.tcrid is null and " . // caracteriza dotação inicial
        " d.mcrid is null " . //
        //
        " ) "
    );
}
// dbg( $where );
// dbg( $where_invalidos, 1 );
if (count($where_invalidos) == 0) {
    $where_invalidos = ' false ';
} else {
    $where_invalidos = implode(" or ", $where_invalidos);
}

// só captura os creditos adicionais que possuam dotação inicial ou cancelamento ou remanejamento
$sql_dot = <<<EOT
SELECT COUNT(1) AS contador
  FROM elabrev.despesaacao d
    INNER JOIN monitora.acao a ON a.acaid = d.acaidloa
    LEFT JOIN public.naturezadespesa n ON n.ndpid = d.ndpid
    LEFT JOIN public.naturezareceita nr ON nr.nrcid = d.nrcid
    INNER JOIN public.idoc i ON i.idoid = d.idoid
    INNER JOIN public.unidade u ON u.unicod = a.unicod AND u.unitpocod = a.unitpocod
  WHERE(({$where}
         AND d.dpavalor > 0
         AND d.tcrid IS NULL
         AND d.mcrid IS NULL)
        OR({$where_invalidos}))
EOT;

$retorno = $db->pegaUm($sql_dot);
$msgregistros = '';
if ($retorno > 100)
    $msgregistros = 'Existem muitos registros para o tipo de crédito selecionado. <br> Por favor, filtre os dados por Programa e/ou Ação e/ou Localizador.';

if (trim($msgregistros) == '') {
    $creditos = cre_pegar_remanejamentos($tcrid, array($unicod), array($prgcod), array($acacod), array($loccod), $ungcod, array($plocod));
} else {
    $creditos = array();
}
//dbg($creditos,1);
// ---------- SQLs PARA ITENS EXTRAS
// captura restrições do tipo de crédito

$where_acaid = array();
$where_gndcod = array();
$sql_acaids =
    " select acaid " .
    " from elabrev.tipocreditoacao " .
    " where tcrid = " . $tcrid .
    " group by acaid ";
$ids = $db->carregar($sql_acaids);
if ($ids) {
    foreach ($ids as $linha) {
        array_push($where_acaid, " a.acaid = " . $linha['acaid']);
    }
}
$sql_gndcods =
    " select gndcod " .
    " from elabrev.tipocreditognd " .
    " where tcrid = " . $tcrid .
    " group by gndcod ";
$cods = $db->carregar($sql_gndcods);
if ($cods) {
    foreach ($cods as $linha) {
        array_push($where_gndcod, " gndcod = " . $linha['gndcod']);
    }
}

$where_gndcod = implode(' or ', $where_gndcod);
$where_acaid = implode(' or ', $where_acaid);

// PROGRAMA (utiliza mesma query para os combos de filtro mais abaixo)
// NATUREZA DE DESPESA

$where_natureza = $where_gndcod ? " and ( " . $where_gndcod . " ) " : '';
$sql_natureza = <<<EOT
		select  ndpcod as codigo, ndpdsc as descricao
		from public.naturezadespesa
		where ndpstatus = 'A'
		and ndpcod ilike '%0000'
		$where_natureza
		group by ndpcod, ndpdsc
		order by codigo
EOT;

// FONTE DE RECURSO


$sql_fonte = "select foncod as codigo, fondsc as descricao from public.fonterecurso where fonstatus = 'A' order by foncod, fondsc";

// IDOC

$sql_idoc = "select idocod as codigo, idodsc as descricao from public.idoc where idostatus = 'A' order by idocod, idodsc";

// IDUSO

$sql_iduso = "select iducod as codigo, idudsc as descricao from public.identifuso where idustatus = 'A' order by iducod, idudsc";

//---------- SQLs PARA COMBOS DE FILTRO
// exibe somente os programas que estão em alguma ação da unidade atual

$ano = $_SESSION['exercicio'];

$wh = "";
// Unidade Gestora
if ($ungcod) {
    $arrPrgcods = $db->carregarColuna("SELECT DISTINCT prgcod FROM elabrev.progugexercicio WHERE ungcod = '" . $ungcod . "' and exercicio = '" . $_SESSION['exercicio'] . "'");
    $wh = " WHERE p.prgcod in ('" . implode("','", $arrPrgcods) . "') ";
}


$sql_prgcod = <<<EOF
		select p.prgcod as codigo, p.prgcod || ' - ' || substr( p.prgdsc, 0, 80 ) as descricao from monitora.programa p
		inner join monitora.acao a on a.prgid=p.prgid and a.unicod = '$unicod' and p.prgano = '$ano'
		inner join elabrev.despesaacao d on d.acaidloa = a.acaid
		$wh
		group by  p.prgcod,  p.prgdsc order by 2
EOF;


$wh = "";
if ((isset($_REQUEST["prgcod"])) && ($_REQUEST["prgcod"] != "")) {
    $wh = "and a.prgcod = '" . $_REQUEST["prgcod"] . "'";
}

// Unidade Gestora
if ($ungcod) {
    $arrAcacods = $db->carregarColuna("SELECT DISTINCT acacod FROM elabrev.progugexercicio WHERE ungcod = '" . $ungcod . "' and exercicio = '" . $_SESSION['exercicio'] . "'");
    $wh .= " AND a.acacod in ('" . implode("','", $arrAcacods) . "') ";
}

//implementando regra de remanejamento entre PO (tipo $tcrcod = TIPO_CREDITO_REMANEJAMENTO_PO)
if ($tcrcod != TIPO_CREDITO_REMANEJAMENTO_PO) {
    $sqlTipoCredito = "inner join ELABREV.ACAOTIPOCREDITO ac on ac.acacod = a.acacod and ac.tcrcod = '$tcrcod' ";
} else {
    $sqlTipoCredito = " ";
}

// exibe somente as ações da unidade atual
$sql_acacod = <<<EOF
		select a.acacod as codigo, a.acacod || ' - ' || substr( a.acadsc, 0, 80 ) as descricao 
		from monitora.acao a
		inner join elabrev.despesaacao d on d.acaidloa = a.acaid
		$sqlTipoCredito
		where a.prgano = '$ano' and a.unicod = '$unicod' $wh group by  a.acacod,  a.acadsc order by 2
EOF;

if ((isset($_REQUEST["acacod"])) && ($_REQUEST["acacod"] != "")) {
    $wh .= " and a.acacod = '" . $_REQUEST["acacod"] . "'";
}

// exibe somente os localizadores que estão em alguma ação da unidade atual
$sql_loccod = <<<EOF
		select a.loccod as codigo, a.loccod || ' - ' || substr( a.sacdsc, 0, 80 ) as descricao 
		from monitora.acao a
		inner join elabrev.despesaacao d on d.acaidloa = a.acaid
		where a.prgano = '$ano' and a.unicod = '$unicod' and a.acastatus = 'A' $wh group by  a.loccod,  a.sacdsc order by 2
EOF;

// exibe somente os localizadores que estão em alguma ação da unidade atual
$sql_irpcod = <<<EOF
		select rp.irpcod as codigo, rp.irpcod || ' - ' || substr( rp.irpdescricao, 0, 180 ) as descricao 
		from elabrev.resultadoprimario rp
		
		where rp.exercicio = '$ano' order by 1
EOF;

// captura nome do momento de crédito
$sql = "select mcrdsc from elabrev.momentocredito where mcrid = " . $_SESSION['credito_mcrid'];
$mcrdsc = $db->pegaUm($sql);

print '<br/>';
$db->cria_aba($abacod_tela, $url, '');
monta_titulo($titulo_modulo, 'Suplementação e Cancelamento de Créditos Adicionais');
?>
<form action="" method="post" name="escolher">
    <input type="hidden" name="escolher" value="1"/>
    <input type="hidden" name="ungcod" value="<?php echo $ungcod; ?>"/>
    <table class="tabela" width="90%" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" style="border-bottom: 0px;">
        <tr>
            <td width="20%" align="right" class="SubTituloDireita">
                Momento:
            </td>
            <td width="80%">
<?= $mcrdsc ?>
            </td>
        </tr>
        <tr>
            <td width="20%" align="right" class="SubTituloDireita">
                Tipo de Crédito:
            </td>
            <td width="80%">
                <input type="hidden" name="tcrid" value="<?php echo $tcrid; ?>"/>
                <b><?= $tcrcod ?></b> - <?= $tcrdsc ?>
            </td>
        </tr>
        <tr>
            <td width="20%" align="right" class="SubTituloDireita">
                Unidade:
            </td>
            <td width="80%">
                <input type="hidden" name="unicod" value="<?php echo $unicod; ?>"/>
                <b><?= $unicod ?></b> - <?= $unidsc ?>
            </td>
        </tr>
<?php if ($ungcod): ?>
    <?php
    $ungdsc = $db->pegaUm("SELECT ungdsc FROM public.unidadegestora WHERE ungcod = '" . $ungcod . "'");
    ?>
            <tr>
                <td width="20%" align="right" class="SubTituloDireita">
                    Unidade Gestora:
                </td>
                <td width="80%">
                    <input type="hidden" name="ungcod" value="<?php echo $ungcod; ?>"/>
                    <b><?= $ungcod ?></b> - <?= $ungdsc ?>
                </td>
            </tr>
<?php endif; ?>
        <tr>
            <td width="20%" align="right" class="SubTituloDireita">
                &nbsp;
            </td>
            <td width="80%">
                <input type="button" onclick="voltarEscolher();" name="voltar" value="Voltar para tela de listagem de Créditos Adicionais" />

                <input type="button" onclick="mostraExtrato();" name="extrato" value="Extrato deste Crédito" />
            </td>
        </tr>
        <tr>
            <td width="20%" align="right" class="SubTituloDireita">
                Programa:
            </td>
            <td width="80%">
<?= $db->monta_combo('prgcod', $sql_prgcod, 'S', 'Programa', 'filtrarCreditos();', '', '', '', '', '') ?>
            </td>
        </tr>
        <tr>
            <td width="20%" align="right" class="SubTituloDireita">
                Ação:
            </td>
            <td width="80%">
<?= $db->monta_combo('acacod', $sql_acacod, 'S', 'Ação', 'filtrarCreditos();', '', '', '', '', '') ?>
            </td>
        </tr>
        <tr>
            <td width="20%" align="right" class="SubTituloDireita">
                Localizador:
            </td>
            <td width="80%">
<?= $db->monta_combo('loccod', $sql_loccod, 'S', 'Localizador', 'filtrarCreditos();', '', '', '', '', '') ?>
            </td>
        </tr>
        <tr>
            <td width="20%" align="right" class="SubTituloDireita">
                &nbsp;
            </td>
            <td width="80%">
                <input type="button" onclick="filtrarCreditos();" name="filtrar" value="Filtrar" />
            </td>
        </tr>
        <tr>
            <td width="100%" colspan="2" align="center" class="SubTituloCentro">
                &nbsp;<?php print '<font color=red>' . $msgregistros . '</font>'; ?>
            </td>
        </tr>
    </table>
</form>
<?php if (count($erro_credito_extra)) : ?>
    <p align="center">
        <b><font color="#ff0000">Dados não gravados</font>! Os dados destacados em vermelho são inconsistentes.</b>
    </p>
<?php endif; ?>
<script type="text/javascript">
	
    function voltarEscolher()
    {
        document.escolher.escolher.value = '1';
        document.escolher.submit();
    }
	
    function filtrarCreditos()
    {
        document.escolher.escolher.value = '0';
        document.escolher.submit();
    }

    function mostraExtrato()
    {
        var url =
            '?modulo=relatorio/remanejamento/credito_unidade&acao=A' +
            '&tcrid=' + '<?= $tcrid ?>' +
            '&unicod=' +  '<?= $unicod ?>' +
            '&mcrid=' +  '<?= $_SESSION['credito_mcrid'] ?>' +
            '&ungcod=' + '<?= $ungcod ?>';
        window.open(
        url,
        'creditosAdicionaisDetalhesUnidade',
        'toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=yes,width=650,height=550'
    );
    }
	
</script>
<script type="text/javascript"><!--
	
    // ----- VARIÁVEIS -----
	
    // utilizado para verificar se existe um item de crédito
    // as chaves são formadas da seguinte forma:
    // prgcod.acacod.unicod.loccod.ndpcod.foncod.idocod.iducod
    var existe = new Array();
	
    /*
        Crédito para cada item da tabela
        Cada índice é o número de uma linha da tabela
            0 => array(
                0 => 0 // saldo inicial
                1 => 0 // cancelamento do item
                2 => 0 // suplementacao do item
                3 => 0 // vlexcesso do item
                4 => 0 // vlsuperavit do item
            )
     */
    var credito = new Array();
	
    // dotação inicial geral
    var dot = 0;
	
    // outros cancelamentos gerais
    var can_out = 0;
	
    // outras suplementações gerais
    var sup_out = 0;

    var vlr_dot = 0;
	
    var span_saldo = null;
	
    var span_can = null;
	
    var span_sup = null;

    var span_vlexcesso = null;

    var span_vlsuperavit = null;
	
    // máxima porcentagem de suplementação
    var MAX_SUP = 0;
	
    // máxima porcentagem de cancelamento
    var MAX_CAN = 0;
	
	
    // ----- FUNÇÕES -----
	
    function formatarNumero( numero )
    {
        var sinal = numero < 0 ? '-' : '';
        numero += '';
        if ( sinal == '-' )
        {
            numero = numero.substring( 1 );
        }
        if ( numero.length < 4 )
        {
            return sinal + numero;
        }
        var casas = new Array();
        var chave = 0;
        for ( var chave = -1, j = 0, i = numero.length - 1; i >= 0; i--, j++ )
        {
            if ( j % 3 == 0 )
            {
                chave++;
                casas[chave] = '';
            }
            casas[chave] = numero.charAt( i ) + casas[chave];
        }
        var resultado = new Array();
        for ( var i = casas.length - 1; i >= 0; i-- )
        {
            resultado[resultado.length] = casas[i];
        }
        return sinal + resultado.join( '.' );
    }

    function pegarInteiro( valor )
    {
        valor += '';
        valor = valor.replace( /\./g, '' );
        while ( valor.charAt( 0 ) == '0' )
        {
            valor = valor.substring( 1 );
        }
        return valor == '' ? 0 : parseInt( valor );
    }
	
    function enviarCreditos()
    {
        document.creditos.submit();
    }
	
    /**
     * Deve ser chamado no momento em que se altera o valor de cancelamento de um item.
     * Esta função altera apenas o valor de cancelamento em memória no javascript.
     *
     * @param integer
     * @param object
     * @return void
     */
    function alterarCan( id, input )
    {
        var valor = pegarInteiro( input.value );
        input.value = formatarNumero( valor );
        credito[id][1] = valor;
        atualizarSaldoItem( id );
        atualizarSomas();
    }
	
    /**
     * Deve ser chamado no momento em que se altera o valor de suplementação de um item
     * Esta função altera apenas o valor de suplementação em memória no javascript.
     *
     * @param integer
     * @param object
     * @return void
     */
    function alterarSup( id, input )
    {
        var valor = pegarInteiro( input.value );
        input.value = formatarNumero( valor );
        credito[id][2] = valor;
        atualizarSaldoItem( id );
        atualizarSomas();
    }

    function alterarvlexcesso( id, input )
    {
        var valor = pegarInteiro( input.value );
        input.value = formatarNumero( valor );
        credito[id][3] = valor;
        atualizarSaldoItem( id );
        atualizarSomas();
    }

    function alterarvlsuperavit( id, input )
    {
        var valor = pegarInteiro( input.value );
        input.value = formatarNumero( valor );
        credito[id][4] = valor;
        atualizarSaldoItem( id );
        atualizarSomas();
    }
	
    function atualizarSomas()
    {
        var can = 0;
        var sup = 0;
        var vlexcesso = 0;
        var vlsuperavit = 0;
        for ( var i = 0, j = credito.length; i < j; i++ )
        {
            // lê cancelamentos e da suplementações dos itens de crédito
            can += credito[i][1];
            sup += credito[i][2];
            vlexcesso += credito[i][3];
            vlsuperavit += credito[i][4];
        }
        var saldo  = (vlexcesso + vlsuperavit);     // inicia saldo com dotação inicial
        //saldo -= can_out; // retira outros cancelamentos
        //saldo += sup_out; // adiciona outras suplementações
        saldo += vlr_dot; // adiciona dotação aprovada
        saldo -= can;     // retira cancelamento dos itens
        saldo += sup;     // adiciona suplementação dos itens
        span_saldo.style.color = saldo < 0 ? '#ff0000' : '#0000ff';
        span_saldo.innerHTML = formatarNumero( saldo );
        span_can.innerHTML = formatarNumero( can );
        span_sup.innerHTML = formatarNumero( sup );
        span_vlexcesso.innerHTML = formatarNumero( vlexcesso );
        span_vlsuperavit.innerHTML = formatarNumero( vlsuperavit );
    }
	
    function atualizarSaldoItem( id )
    {
        var saldo = calcularSaldoItem( id );
        var span = document.getElementById( 'credito_saldo_' + id );
        span.style.color = saldo < 0 ? '#ff0000' : '#0000ff';
        span.innerHTML = mascaraglobal('###.###.###.###.###', Math.round(saldo));
    }
	
    /**
     * Calcula o saldo atual de um item.
     *
     * @param integer
     * @return integer
     */
    function calcularSaldoItem( id )
    {
        var saldo  = credito[id][0]; // inicia com saldo atual
        saldo -= credito[id][1]; // retira cancelamento do item
        saldo += credito[id][2]; // adiciona suplementação do item
        saldo += credito[id][3]; // adiciona valor excesso do item
        saldo += credito[id][4]; // adiciona valor superavit do item
	    
        return saldo;
    }
	
    // ---------- FUNÇÕES PARA MANIPULAÇÂO DE ITENS NOVOS (despesas que não existiam inicialmente)
	
    function novoItem_moveProximoCampo( event, campo )
    {
        if ( event.keyCode != 13 )
        {
            return;
        }
        switch ( campo )
        {
            case 'programa':
                var input = document.getElementById( 'novo_acacod' );
                break;
            case 'acao':
                var input = document.getElementById( 'novo_loccod' );
                break;
            case 'localizador':
                var input = document.getElementById( 'novo_ndpcod' );
                break;
            case 'plocod':
                var input = document.getElementById( 'novo_plocod' );
                break;
            case 'natureza':
                var input = document.getElementById( 'novo_foncod' );
                break;
            case 'fonte':
                var input = document.getElementById( 'novo_idocod' );
                break;
            case 'idoc':
                var input = document.getElementById( 'novo_iducod' );
                break;
            case 'iduso':
                var input = document.getElementById( 'novo_sup' );
                break;
            case 'irpcod':
                var input = document.getElementById( 'novo_irpcod' );
                break;
            case 'sup':
                var input = document.getElementById( 'novo_can' );
                break;
            case 'can':
                var input = document.getElementById( 'novo_vlexcesso' );
                break;
            case 'vlexcesso':
                var input = document.getElementById( 'novo_vlsuperavit' );
                break;
            case 'vlsuperavit':
                var input = novoItem_inserirDoFormulario();
                if ( !input )
                {
                    return;
                }
                break;
            default:
                return;
        }
        input.focus();
        input.select();
    }

    /**
     * Verifica todos as linhas da tabela em busca de uma ocorrência da mova tupla
     */
    function checaUnicidade(dadosCredito)
    {
        var result = true;
        jQuery('tr[id^="credito_"]').each(function(){
            var _id = '#' + jQuery(this).attr('id');
            var encontrouLinha = true;
            for (_key in dadosCredito) {
                var seletor = _id + ' td.coluna' + _key + '_credito:contains(' + dadosCredito[_key].toUpperCase() + ')';
                encontrouLinha = encontrouLinha && (0 != jQuery(seletor).size());
            }
            if (encontrouLinha) {
                setTimeout(function(color){
                    jQuery(_id).css('background-color', color);
                }, 5000, jQuery(_id).css('background-color'));
                jQuery(_id).css('background-color', 'MediumSeaGreen');
                jQuery(_id + ' input:first').select().focus();
                jQuery('div.div_rolagem').animate(
                {scrollTop: jQuery(_id).offset().top - 100},
                500
            );
                result = false;
                return false;
            }
        });
        return result;
    }

    function novoItem_inserirDoFormulario()
    {
        var prgcod = document.getElementById('novo_prgcod').value;
        var acacod = document.getElementById('novo_acacod').value;
        var loccod = document.getElementById('novo_loccod').value;
        var ndpcod = document.getElementById('novo_ndpcod').value;
        var plocod = document.getElementById('novo_plocod').value;
        var foncod = document.getElementById('novo_foncod').value;
        var idocod = document.getElementById('novo_idocod').value;
        var iducod = document.getElementById('novo_iducod').value;
        var irpcod = document.getElementById('novo_irpcod').value;
        var novo_sup = document.getElementById('novo_sup').value;
        var novo_can = document.getElementById('novo_can').value;
        var novo_vlexcesso = document.getElementById('novo_vlexcesso').value;
        var novo_vlsuperavit = document.getElementById('novo_vlsuperavit').value;
        if (!prgcod) {
            alert('Preencha o Programa');
            return document.getElementById('novo_prgcod');
        }
        if (!acacod) {
            alert('Preencha a Ação');
            return document.getElementById('novo_acacod');
        }
        if (!loccod) {
            alert('Preencha a Localização');
            return document.getElementById('novo_loccod');
        }
        if (!ndpcod) {
            alert('Preencha a Natureza');
            return document.getElementById('novo_ndpcod');
        }
        if (!foncod) {
            alert('Preencha a Fonte');
            return document.getElementById('novo_foncod');
        }
        if (!idocod) {
            alert('Preencha o IDOC');
            return document.getElementById('novo_idocod');
        }
        if (!iducod) {
            alert('Preencha O IDUSO');
            return document.getElementById('novo_iducod');
        }
        // -- Validação de existência de outra tupla com os mesmos dados (de Prog. a RP)
        var dadosCredito = {
            '1': prgcod,
            '2': acacod,
            '3': loccod,
            'PO': plocod,
            '4': ndpcod,
            '5': foncod,
            '6': idocod,
            '7': iducod,
            'RP': irpcod
        };
        if (!checaUnicidade(dadosCredito)) {
            alert('A programática informada já existe.');
            return false;
        }

        /*
        // CRÍTICAS PARA OPERAÇÕES DE CRÉDITO.
        if(idocod == 9999) {
            // Testa o ERRO 35
            if((iducod == 1) || (iducod == 2) || (iducod == 3)) {
                alert("IDUSO incompatível com IDOC.");
                return document.getElementById( 'novo_iducod' );
            }
            // Testa o ERRO 36
            else if((foncod.toString().substring(1) == '46') || (foncod.toString().substring(1) == '47') ||
                   (foncod.toString().substring(1) == '48') || (foncod.toString().substring(1) == '49') ||
                   (foncod.toString().substring(1) == '91')) {
                alert("Fonte incompatível com IDOC.");
                return document.getElementById( 'novo_foncod' );
            }
            // Testa o ERRO 37
            else if(((acacod.toString() == '0283') || (acacod.toString() == '0284')) && (iducod == 0) &&
                    ((foncod.toString().substring(1) == '46') || (foncod.toString().substring(1) == '47') ||
                    (foncod.toString().substring(1) == '48') || (foncod.toString().substring(1) == '49') ||
                    (foncod.toString().substring(1) == '95') || (foncod.toString().substring(1) == '96'))) {
                alert('Fonte incompatível com Ação.');
                return document.getElementById( 'novo_acacod' );
            }
            // Testa o ERRO 38
            else if(((acacod.toString() != '0283') && (acacod.toString() != '0284')) && (iducod == 0) &&
                    ((foncod.toString().substring(1) != '46') && (foncod.toString().substring(1) != '47') &&
                    (foncod.toString().substring(1) != '48') && (foncod.toString().substring(1) != '49') &&
                    (foncod.toString().substring(1) != '95') && (foncod.toString().substring(1) != '96'))) {
                alert("IDUSO/Fonte incompatível com IDOC.");
                return document.getElementById( 'novo_iducod' );
            }
            // Testa o ERRO 39
            else if(((iducod == 1) || (iducod == 2) || (iducod == 3)) &&
                    ((foncod.toString().substring(1) == '46') && (foncod.toString().substring(1) == '47') &&
                    (foncod.toString().substring(1) == '48') && (foncod.toString().substring(1) == '49') &&
                    (foncod.toString().substring(1) == '91') && (foncod.toString().substring(1) == '94') &&
                    (foncod.toString().substring(1) == '95') && (foncod.toString().substring(1) == '96'))) {
                alert("IDUSO incompatível com Fonte.");
                return document.getElementById( 'novo_iducod' );
            }
        }
         */
		
        var chave =
            prgcod + '.' + acacod + '.' +
            loccod + '.' + plocod + '.' + ndpcod + '.' +
            foncod + '.' + idocod + '.' +
            iducod + '.' + irpcod;
			
        if ( existe[chave] )
        {
            alert( 'Item de crédito já está em uso' );
            var input = document.getElementById( 'item_sup_' + chave );
            input.focus();
            input.select();
            return null;
        }
        novoItem_inserir( prgcod, acacod, loccod, plocod, ndpcod, foncod, idocod, iducod, irpcod, novo_sup, novo_can, novo_vlexcesso, novo_vlsuperavit, new Array() );
        document.getElementById( 'novo_prgcod' ).value = '';
        document.getElementById( 'novo_acacod' ).value = '';
        document.getElementById( 'novo_plocod' ).value = '';
        document.getElementById( 'novo_loccod' ).value = '';
        document.getElementById( 'novo_ndpcod' ).value = '';
        document.getElementById( 'novo_foncod' ).value = '';
        document.getElementById( 'novo_idocod' ).value = '';
        document.getElementById( 'novo_iducod' ).value = '';
        document.getElementById( 'novo_irpcod' ).value = '';
        document.getElementById( 'novo_sup' ).value = '0';
        document.getElementById( 'novo_can' ).value = '0';
        document.getElementById( 'novo_vlexcesso' ).value = '0';
        document.getElementById( 'novo_vlsuperavit' ).value = '0';
        var prg = document.getElementById( 'novo_prgcod' );
        prg.focus();
        prg.select();
        atualizarSomas();
        return null;
    }
	
    function novoItem_criarCelula( tr, nome, valor, align, cor, classe )
    {
        var celula = tr.insertCell( tr.cells.length );
        //celula.style.textAlign = align;
        celula.style.color = cor;
        celula.className = classe;
        celula.appendChild( document.createTextNode( valor ) );
        celula.appendChild( novoItem_CriarHidden( 'extra[' + nome + '][]', valor ) );
    }
	
    function novoItem_CriarHidden( nome, valor )
    {
        var input = document.createElement( 'input' );
        input.type = 'hidden';
        input.name = nome;
        input.value = valor;
        return input;
    }
	
    function novoItem_povoarFormulario( prg, aca, loc, plo, ndp, fon, ido, idu, irp, sup, can, vlexcesso, vlsuperavit )
    {
        document.getElementById( 'novo_prgcod' ).value = prg;
        document.getElementById( 'novo_acacod' ).value = aca;
        document.getElementById( 'novo_plocod' ).value = plo;
        document.getElementById( 'novo_loccod' ).value = loc;
        document.getElementById( 'novo_ndpcod' ).value = ndp;
        document.getElementById( 'novo_foncod' ).value = fon;
        document.getElementById( 'novo_idocod' ).value = ido;
        document.getElementById( 'novo_iducod' ).value = idu;
        document.getElementById( 'novo_irpcod' ).value = irp;
		
        var inputSup = document.getElementById( 'novo_sup' );
        inputSup.value = sup;
        inputSup.focus();
        inputSup.select();
        var inputCan = document.getElementById( 'novo_can' );
        inputCan.value = can;
        inputCan.focus();
        inputCan.select();
        var inputvlexcesso = document.getElementById( 'novo_vlexcesso' );
        inputvlexcesso.value = vlexcesso;
        inputvlexcesso.focus();
        inputvlexcesso.select();
        var inputvlsuperavit = document.getElementById( 'novo_vlsuperavit' );
        inputvlsuperavit.value = vlsuperavit;
        inputvlsuperavit.focus();
        inputvlsuperavit.select();
    }
	
    function novoItem_editar( id, chave )
    {
        var prgcod = document.getElementById( 'novo_prgcod' ).value;
        var acacod = document.getElementById( 'novo_acacod' ).value;
        var plocod = document.getElementById( 'novo_plocod' ).value;
        var loccod = document.getElementById( 'novo_loccod' ).value;
        var ndpcod = document.getElementById( 'novo_ndpcod' ).value;
        var foncod = document.getElementById( 'novo_foncod' ).value;
        var idocod = document.getElementById( 'novo_idocod' ).value;
        var iducod = document.getElementById( 'novo_iducod' ).value;
        var irpcod = document.getElementById( 'novo_irpcod' ).value;
        if ( prgcod || acacod || loccod || ndpcod || foncod || idocod || iducod )
        {
            if ( !confirm( 'Deseja sobrepor os dados de crédito extra?' ) )
            {
                return;
            }
        }

        var sup = document.getElementById( 'item_sup_' + chave ).value;
        var can = document.getElementById( 'item_can_' + chave ).value;
        var vlexcesso = document.getElementById( 'item_vlexcesso_' + chave ).value;
        var vlsuperavit = document.getElementById( 'item_vlsuperavit_' + chave ).value;
		
        var dados = chave.split( '.' );

        novoItem_remover( id, chave );
		
        novoItem_povoarFormulario( dados[0], dados[1], dados[2], dados[3], dados[4], dados[5], dados[6],dados[7], dados[8], sup, can, vlexcesso, vlsuperavit );
    }
	
    function novoItem_remover( id, chave )
    {
        existe[chave] = false;
        credito[id] = new Array( 0, 0, 0, 0, 0 );
        var tr = document.getElementById( 'novoitem_' + id );
        if ( tr )
        {
            tr.parentNode.removeChild( tr );
        }
        atualizarSomas();
        novoItem_AtualizarCor();
    }
	
    function novoItem_inserir( prgcod, acacod, loccod, plocod, ndpcod, foncod, idocod, iducod, irpcod, sup_valor, can_valor, vlexcesso_valor, vlsuperavit_valor, erro )
    {
        var tabela = document.getElementById( 'tabela_credito' );
		
        var chave =
            prgcod + '.' + acacod + '.' +
            loccod + '.' + plocod + '.' + ndpcod + '.' +
            foncod + '.' + idocod + '.' +
            iducod + '.' + irpcod;
		
        var id = credito.length;
		
        existe[chave] = true;
        credito[id] = new Array( 0, pegarInteiro( can_valor ), pegarInteiro( sup_valor ), pegarInteiro( vlexcesso_valor ), pegarInteiro( vlsuperavit_valor ) );
		
        // cria linha
        var tr = tabela.insertRow( tabela.rows.length );
        tr.id = 'novoitem_' + id;
		
        // cria celula a célula de cada campo
        var cor = '';
		
        cor = erro['prg'] ? '#ff0000' : '#000000';
        novoItem_criarCelula( tr, 'prgcod', prgcod, 'center', cor, 'coluna1_credito' );
		
        cor = erro['aca'] ? '#ff0000' : '#000000';
        novoItem_criarCelula( tr, 'acacod', acacod, 'center', cor, 'coluna2_credito' );
		
        cor = erro['loc'] ? '#ff0000' : '#000000';
        novoItem_criarCelula( tr, 'loccod', loccod, 'center', cor, 'coluna3_credito' );
		
        cor = erro['plo'] ? '#ff0000' : '#000000';
        novoItem_criarCelula( tr, 'plocod', plocod, 'center', cor, 'colunaPO_credito' );
		
        cor = erro['ndp'] ? '#ff0000' : '#000000';
        novoItem_criarCelula( tr, 'ndpcod', ndpcod, 'center', cor, 'coluna4_credito' );
		
        cor = erro['fon'] ? '#ff0000' : '#000000';
        novoItem_criarCelula( tr, 'foncod', foncod, 'center', cor, 'coluna5_credito' );
		
        cor = erro['ido'] ? '#ff0000' : '#000000';
        novoItem_criarCelula( tr, 'idocod', idocod, 'center', cor, 'coluna6_credito' );
		
        cor = erro['idu'] ? '#ff0000' : '#000000';
        novoItem_criarCelula( tr, 'iducod', iducod, 'center', cor, 'coluna7_credito' );
		
        cor = erro['irp'] ? '#ff0000' : '#000000';
        novoItem_criarCelula( tr, 'irpcod', irpcod, 'center', cor, 'colunaRP_credito' );
		
        var celulaDot = tr.insertCell( tr.cells.length );
        //celulaDot.style.textAlign = 'right';
        celulaDot.className = 'coluna8_credito';
        celulaDot.appendChild( document.createTextNode( '0' ) );
			
        var celulaSupOut = tr.insertCell( tr.cells.length );
        celulaSupOut.style.textAlign = 'right';
        celulaSupOut.style.color = '#000050';
        celulaSupOut.className = 'coluna9_credito';
        celulaSupOut.appendChild( document.createTextNode( '0' ) );
		
        //		var celulaSupOut = tr.insertCell( tr.cells.length );
        //			//celulaSupOut.style.textAlign = 'right';
        //			celulaSupOut.style.color = '#000050';
        //			celulaSupOut.className = 'coluna9_credito';
        //			celulaSupOut.appendChild( document.createTextNode( '0' ) );
        //
        //		var celulaCanOut = tr.insertCell( tr.cells.length );
        //			//celulaCanOut.style.textAlign = 'right';
        //			celulaCanOut.style.color = '#500000';
        //			celulaCanOut.className = 'coluna10_credito';
        //			celulaCanOut.appendChild( document.createTextNode( '0' ) );
		
        var inputSup = document.createElement( 'input' );
        inputSup.id = 'item_sup_' + chave;
        inputSup.type = 'text';
        inputSup.name = 'extra[sup][]';
        inputSup.value = sup_valor;
        inputSup.style.textAlign = 'right';
        //inputSup.style.color = '#000050';
        //inputSup.size = '13';
        inputSup.className = 'normal';
        var keyup = new Function ( ' alterarSup( \'' + id + '\', this ); ' );
        if ( navigator.userAgent.indexOf("Safari") > 0 )
        {
            inputSup.addEventListener( "keyup", keyup, false );
        }
        else if ( navigator.product == "Gecko" )
        {
            inputSup.addEventListener( "keyup", keyup, false );
        }
        else
        {
            inputSup.onKeyUp = new Function( '' );
            inputSup.attachEvent( 'onkeyup', keyup );
        }

        var inputCan = document.createElement( 'input' );
        inputCan.id = 'item_can_' + chave;
        inputCan.type = 'text';
        inputCan.name = 'extra[can][]';
        inputCan.value = can_valor;
        inputCan.style.textAlign = 'right';
        //inputCan.style.color = '#000050';
        //inputCan.size = '13';
        inputCan.className = 'normal';
        var keyup = new Function ( ' alterarCan( \'' + id + '\', this ); ' );
        if ( navigator.userAgent.indexOf("Safari") > 0 )
        {
            inputCan.addEventListener( "keyup", keyup, false );
        }
        else if ( navigator.product == "Gecko" )
        {
            inputCan.addEventListener( "keyup", keyup, false );
        }
        else
        {
            inputCan.onKeyUp = new Function( '' );
            inputCan.attachEvent( 'onkeyup', keyup );
        }

        var inputvlexcesso = document.createElement( 'input' );
        inputvlexcesso.id = 'item_vlexcesso_' + chave;
        inputvlexcesso.type = 'text';
        inputvlexcesso.name = 'extra[vlexcesso][]';
        inputvlexcesso.value = vlexcesso_valor;
        inputvlexcesso.style.textAlign = 'right';
        //inputvlexcesso.style.color = '#000050';
        //inputvlexcesso.size = '13';
        inputvlexcesso.className = 'normal';
        var keyup = new Function ( ' alterarvlexcesso( \'' + id + '\', this ); ' );
        if ( navigator.userAgent.indexOf("Safari") > 0 )
        {
            inputvlexcesso.addEventListener( "keyup", keyup, false );
        }
        else if ( navigator.product == "Gecko" )
        {
            inputvlexcesso.addEventListener( "keyup", keyup, false );
        }
        else
        {
            inputvlexcesso.onKeyUp = new Function( '' );
            inputvlexcesso.attachEvent( 'onkeyup', keyup );
        }

        var inputvlsuperavit = document.createElement( 'input' );
        inputvlsuperavit.id = 'item_vlsuperavit_' + chave;
        inputvlsuperavit.type = 'text';
        inputvlsuperavit.name = 'extra[vlsuperavit][]';
        inputvlsuperavit.value = vlsuperavit_valor;
        inputvlsuperavit.style.textAlign = 'right';
        //inputvlsuperavit.style.color = '#000050';
        //inputvlsuperavit.size = '13';
        inputvlsuperavit.className = 'normal';
        var keyup = new Function ( ' alterarvlsuperavit( \'' + id + '\', this ); ' );
        if ( navigator.userAgent.indexOf("Safari") > 0 )
        {
            inputvlsuperavit.addEventListener( "keyup", keyup, false );
        }
        else if ( navigator.product == "Gecko" )
        {
            inputvlsuperavit.addEventListener( "keyup", keyup, false );
        }
        else
        {
            inputvlsuperavit.onKeyUp = new Function( '' );
            inputvlsuperavit.attachEvent( 'onkeyup', keyup );
        }
			
        var celulaSup = tr.insertCell( tr.cells.length );
        //celulaSup.style.textAlign = 'right';
        celulaSup.className = 'coluna11_credito';
        celulaSup.appendChild( inputSup );
		
        var celulaCan = tr.insertCell( tr.cells.length );
        celulaCan.style.color = '#500000';
        //celulaCan.style.textAlign = 'right';
        celulaCan.className = 'coluna12_credito';
        celulaCan.appendChild( inputCan );

        var celulavlexcesso = tr.insertCell( tr.cells.length );
        //celulaSup.style.textAlign = 'right';
        celulavlexcesso.className = 'coluna13_credito';
        celulavlexcesso.appendChild( inputvlexcesso );

        var celulavlsuperavit = tr.insertCell( tr.cells.length );
        //celulaSup.style.textAlign = 'right';
        celulavlsuperavit.className = 'coluna14_credito';
        celulavlsuperavit.appendChild( inputvlsuperavit );
		
        var imgEditar = document.createElement( 'img' );
        imgEditar.src = '/imagens/check_p.gif';
        imgEditar.style.border = 'none';
        imgEditar.style.padding = '0px';
        imgEditar.style.margin = '0px';
        imgEditar.alt = 'Alterar crédito extra';
        imgEditar.title = 'Alterar crédito extra';
        imgEditar.onclick = "";
        var imgDeletar = document.createElement( 'img' );
        imgDeletar.src = '/imagens/exclui_p.gif';
        imgDeletar.style.border = 'none';
        imgDeletar.style.padding = '0px';
        imgDeletar.style.margin = '0px';
        imgDeletar.alt = 'Excluir crédito extra';
        imgDeletar.title = 'Excluir crédito extra';
        var linkEditar = document.createElement( 'a' );
        linkEditar.href = 'javascript:novoItem_editar( \'' + id + '\', \'' + chave + '\' );';
        linkEditar.title = 'remover';
        linkEditar.style.color = '#ff0000';
        linkEditar.appendChild( imgEditar );
        var linkDeletar = document.createElement( 'a' );
        linkDeletar.href = 'javascript:novoItem_remover( \'' + id + '\', \'' + chave + '\' );';
        linkDeletar.title = 'remover';
        linkDeletar.style.color = '#ff0000';
        linkDeletar.appendChild( imgDeletar );
        var celulaAcao = tr.insertCell( tr.cells.length );
        //celulaAcao.style.textAlign = 'center';
        celulaAcao.style.width = '4%';
        celulaAcao.appendChild( linkEditar );
        celulaAcao.appendChild( document.createTextNode( ' ' ) );
        celulaAcao.appendChild( linkDeletar );
			
        sup_valor = replaceAll(sup_valor, '.', '');
        vlexcesso_valor = replaceAll(vlexcesso_valor, '.', '');
        vlsuperavit_valor = replaceAll(vlsuperavit_valor, '.', '');
        can_valor = replaceAll(can_valor, '.', '');
		
        var SaldoLinha = ( parseFloat(sup_valor) + parseFloat(vlexcesso_valor) + parseFloat(vlsuperavit_valor) ) - parseFloat(can_valor);

        var spanSaldo = document.createElement( 'span' );
        spanSaldo.id = 'credito_saldo_' + id;
        spanSaldo.style.color = '#0000ff';
        spanSaldo.appendChild( document.createTextNode( formatarNumero(SaldoLinha) ) );
        var celulaSaldo = tr.insertCell( tr.cells.length );
        celulaSaldo.style.color = '#500000';
        //celulaSaldo.style.textAlign = 'right';
        celulaSaldo.appendChild( spanSaldo );
        celulaSaldo.style.width = '4%';
		
        atualizarSomas();
        novoItem_AtualizarCor();
    }
	
    function novoItem_AtualizarCor()
    {
        var tabela = document.getElementById( 'tabela_credito' );
        var quantidade_tr = tabela.rows.length;
        var cor = '';
        for ( var i = 0; i < quantidade_tr; i++ )
        {
            var tr = tabela.rows[i];
            cor = cor == '' ? '#e9e9e9' : '' ;
            tr.style.backgroundColor = cor;
            tr.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
            tr.onmouseout = new Function( ' this.style.backgroundColor = "' + cor + '"; ' );
        }
    }
	
    function filtraPO(acao){
	
        jQuery.ajax({
            type: "POST",
            url: 'elabrev.php?modulo=principal/remanejamento/credito&acao=<?= $_REQUEST['acao'] ?>',
            data: "requisicao=filtraPO&acaoPO="+acao,
            async: false,
            success: function(conteudo){
	   		
                $('#tdPO').html(conteudo);
            }
        });
    }
	
    function verificaRemanejamentoPOSuplementacao( valor, funcional ){
	
        var elementos = $('[funcional=can_'+funcional+']');

        if( valor != 0 ){
            jQuery.each(elementos, function(i, input) {
		     	
                $(input).css("background-color","#D3D3D3");
                $(input).css("color","grey");
                $(input).attr("readonly", true);
                $(input).attr("title", "O remanejamento entre PO's não pode ser feito neste tipo de crédito");
            });
        }else{
            jQuery.each(elementos, function(i, input) {
		     	
                $(input).css("background-color","#fff");
                $(input).css("color","black");
                $(input).attr("readonly", false);
                $(input).attr("title", "");
            });
        }
    }
	
    function verificaRemanejamentoPOCancelamento( valor, funcional ){
	
        var elementos = $('[funcional=sup_'+funcional+']');
        var elementos_sup_ex = $('[funcional=sup_ex_'+funcional+']');
        var elementos_sup_s = $('[funcional=sup_s_'+funcional+']');

        if( valor != 0 ){
            jQuery.each(elementos, function(i, input) {
		     	
                $(input).css("background-color","#D3D3D3");
                $(input).css("color","grey");
                $(input).attr("readonly", true);
                $(input).attr("title", "O remanejamento entre PO's não pode ser feito neste tipo de crédito");
            });
            jQuery.each(elementos_sup_ex, function(i, input) {
		     	
                $(input).css("background-color","#D3D3D3");
                $(input).css("color","grey");
                $(input).attr("readonly", true);
                $(input).attr("title", "O remanejamento entre PO's não pode ser feito neste tipo de crédito");
            });
            jQuery.each(elementos_sup_s, function(i, input) {
		     	
                $(input).css("background-color","#D3D3D3");
                $(input).css("color","grey");
                $(input).attr("readonly", true);
                $(input).attr("title", "O remanejamento entre PO's não pode ser feito neste tipo de crédito");
            });
        }else{
            jQuery.each(elementos, function(i, input) {
		     	
                $(input).css("background-color","#fff");
                $(input).css("color","black");
                $(input).attr("readonly", false);
                $(input).attr("title", "");
            });
            jQuery.each(elementos_sup_ex, function(i, input) {
		     	
                $(input).css("background-color","#fff");
                $(input).css("color","black");
                $(input).attr("readonly", false);
                $(input).attr("title", "");
            });
            jQuery.each(elementos_sup_s, function(i, input) {
		     	
                $(input).css("background-color","#fff");
                $(input).css("color","black");
                $(input).attr("readonly", false);
                $(input).attr("title", "");
            });
        }
    }
	
    --></script>
<style>
    .coluna1_credito
    {
        width: 45px;
    }
    .coluna2_credito
    {
        width: 45px;
    }
    .coluna3_credito
    {
        width: 45px;
    }
    .colunaPO_credito
    {
        width: 45px;
    }
    .colunaRP_credito
    {
        width: 45px;
    }
    .coluna4_credito
    {
        width: 60px;
    }
    .coluna5_credito
    {
        width: 40px;
    }
    .coluna6_credito
    {
        width: 45px;
    }
    .coluna7_credito
    {
        width: 35px;
    }
    .coluna8_credito
    {
        width: 60px;
    }
    .coluna9_credito
    {
        width: 60px;
    }
    .coluna10_credito
    {
        width: 60px;
    }
    .coluna11_credito
    {
        width: 75px;
    }
    .coluna12_credito
    {
        width: 75px;
    }
    .coluna13_credito
    {
        width: 75px;
    }
    .coluna14_credito
    {
        width: 75px;
    }
    .coluna15_credito
    {
        width: 8%;
    }
    .coluna16_credito
    {
        width: 80px;
    }
    .tabela_credito
    {
        color:101010;
        border: 0px;
        padding: 0px;
        border-collapse: collapse;
        margin: 0px;
        table-row: collapse;
        width:95%;
    }
    .tabela_credito td
    {
        font-size: 9px;
        text-align:right;
    }
    .tabela_credito input
    {
        font-size: 8.5px;
        font-family: Tahoma;
    }
    .tabela_credito .coluna11_credito input
    {
        width: 70px;
    }
    .tabela_credito .coluna12_credito input
    {
        width: 70px;
    }
    .tabela_credito .coluna13_credito input
    {
        width: 70px;
    }
    .tabela_credito .coluna14_credito input
    {
        width: 70px;
    }
    .tabela_credito .coluna7_credito input
    {
        width: 15px;
    }
    .barra
    {
        width: 16px;
    }
</style>


<?php if (trim($msgregistros) == '') { ?>
    <form action="" method="post" name="creditos">
        <input type="hidden" name="remanejar" value="1" />
        <input type="hidden" name="tcrid" value="<?php echo $tcrid; ?>" />
        <input type="hidden" name="unicod" value="<?php echo $unicod; ?>" />
        <input type="hidden" name="prgcod" value="<?php echo $prgcod; ?>" />
        <input type="hidden" name="acacod" value="<?php echo $acacod; ?>" />
        <input type="hidden" name="loccod" value="<?php echo $loccod; ?>" />
        <input type="hidden" name="ungcod_creditos" value="<?php echo $ungcod; ?>" />
        <input type="hidden" name="plocod" value="<?php echo $plocod; ?>" />
        <center>
            <table border="0" cellspacing="0" cellpadding="0" class="tabela_credito">
                <thead>
                    <tr bgcolor="#dfdfdf">
                        <td align="center" class="coluna1_credito" title="Programa">Prog.</td>
                        <td align="center" class="coluna2_credito" title="Ação">Ação</td>
                        <td align="center" class="coluna3_credito" title="Localizador">Loc.</td>
                        <td align="center" class="colunaPO_credito" title="Plano Orçamentário">PO</td>
                        <td align="center" class="coluna4_credito" title="Natureza">Natureza</td>
                        <td align="center" class="coluna5_credito" title="Fonte de Recurso">Fonte</td>
                        <td align="center" class="coluna6_credito" title="IDOC">IDOC</td>
                        <td align="center" class="coluna7_credito" title="IDUSO">IDUSO</td>
                        <td align="center" class="colunaRP_credito" title="IDUSO">RP</td>
                        <td align="center" class="coluna8_credito" title="Dotação Inicial">Dot. Inicial</td>
                        <td align="center" class="coluna9_credito" title="Lei+Créditos">Lei+Créditos</td>
                        <!--
                        <td align="center" class="coluna9_credito" title="Outras Suplementações">Outras Sup.</td>
                        <td align="center" class="coluna10_credito" title="Outros Cancelamentos">Outros Can.</td>
                        -->
                        <td align="center" class="coluna11_credito" title="Suplementações">Supl.</td>
                        <td align="center" class="coluna12_credito" title="Cancelamentos">Cancel.</td>
                        <td align="center" class="coluna13_credito" title="Suplementações por Excesso">Supl. por Excesso</td>
                        <td align="center" class="coluna14_credito" title="Suplementações por Superávit">Supl. por Superávit</td>
                        <td align="center" class="coluna15_credito" title="Saldo">Saldo</td>
                        <td align="center" class="barra">&nbsp;</td>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td colspan="16">
                            <div style="margin:0;padding:0;height:320px;width:100%;" class="div_rolagem">
                                <table border="0" width="100%" cellspacing="0" cellpadding="0" style="color:#101010;">
                                    <tbody style="color:#505050;font-size:7px;" id="tabela_credito">
    <?php $dot = 0; ?>
    <?php $can_out = 0; ?>
    <?php $sup_out = 0; ?>
    <?php $vlr_dot = 0; ?>
    <?php $can = 0; ?>
    <?php $sup = 0; ?>
    <?php $vlexcesso = 0; ?>
    <?php $vlsuperavit = 0; ?>
    <?php $cor = '#ffffff'; ?>
    <?php foreach ($creditos as $id => $credito) : ?>
        <?php $cor = $cor == '#ffffff' ? '#f0f0f0' : '#ffffff'; ?>
                                            <tr id="credito_<?php echo $id; ?>" bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
                                                <td class="coluna1_credito" title="Programa">
                                                    <script type="text/javascript">

                                                        // prgcod.acacod.unicod.loccod.ndpcod.foncod.idocod.iducod
        <?php
        $chave =
            $credito['prgcod'] . '.' .
            $credito['acacod'] . '.' .
            $credito['loccod'] . '.' .
            $credito['plocod'] . '.' .
            $credito['ndpcod'] . '.' .
            $credito['foncod'] . '.' .
            $credito['idocod'] . '.' .
            $credito['iducod'];
        ?>
                                                            existe['<?php echo $chave; ?>'] = true;

                                                            credito[<?php echo $id; ?>] = new Array(
        <?php echo $credito['saldo_out'] ? $credito['saldo_out'] : 0; ?>,
        <?php echo $credito['can'] ? $credito['can'] : 0; ?>,
        <?php echo $credito['sup'] ? $credito['sup'] : 0; ?>,
        <?php echo $credito['vlexcesso'] ? $credito['vlexcesso'] : 0; ?>,
        <?php echo $credito['vlsuperavit'] ? $credito['vlsuperavit'] : 0; ?>
                                                            );
                                                    </script>
        <?php echo $credito['prgcod']; ?>
                                                </td>
                                                <td class="coluna2_credito" title="Ação"><?php echo $credito['acacod']; ?></td>
                                                <td class="coluna3_credito" title="Localizador"><?php echo $credito['loccod']; ?></td>
        <?php
        $sql = "select plotitulo from elabrev.planoorcamentario where plocodigo = '" . $credito['plocod'] . "' and acaid = " . $credito['acacod'];

        $sql = "select  po.titulo as titulo from elabrev.ws_planosorcamentariosdto as po
													join elabrev.ws_acaodto as a on a.identificadorunico = po.identificadorunicoacao 
													where a.codigoacao = '" . $credito['acacod'] . "'
													and a.codigoorgao = '" . $credito['unicod'] . "'
													and po.planoorcamentario = '" . $credito['plocod'] . "'";

        $plotitulo = $db->pegaUm($sql);
        ?>
                                                <td class="colunaPO_credito" title="<?php echo $plotitulo; ?>"><?php echo $credito['plocod']; ?></td>
                                                <td class="coluna4_credito" title="Natureza">
        <?php if ($credito['ndpcod']) : ?>
            <?php echo $credito['ndpcod']; ?>
        <?php else : ?>
            <?php echo $credito['nrccod']; ?>
        <?php endif; ?>
                                                </td>
                                                <td class="coluna5_credito" title="Fonte de Recurso"><?php echo $credito['foncod']; ?></td>
                                                <td class="coluna6_credito" title="IDOC"><?php echo $credito['idocod']; ?></td>
                                                <td class="coluna7_credito" title="IDUSO"><?php echo $credito['iducod']; ?></td>
                                                <td class="colunaRP_credito" title="<?php echo $credito['irpdescricao']; ?>"><?php echo $credito['irpcod']; ?></td>
                                                <td class="coluna8_credito"style="color:#000000;" title="Dotação Inicial">
        <?php echo number_format($credito['dot'], 0, ',', '.'); ?>
                                                </td>

                                                <td class="coluna9_credito" style="color:#000050;" title="Lei+Créditos">
        <?php echo number_format($credito['vlrdot'], 0, ',', '.'); ?>
                                                </td>

                                                <!--
                                                <td class="coluna9_credito" style="color:#000050;" title="Outras Supementações">
        <?php echo number_format($credito['sup_out'], 0, ',', '.'); ?>
                                                </td>
                                                <td class="coluna10_credito" style="color:#500000;" title="Outros Cancelamentos">
        <?php echo number_format($credito['can_out'], 0, ',', '.'); ?>
                                                </td>
                                                -->
                                                <td class="coluna11_credito" title="Suplementação">
        <?php
        $arrFuncional = explode(".", $chave);

        $funcional = $arrFuncional[0]
            . '.' . $arrFuncional[1]
            . '.' . $arrFuncional[2]
            . '.' . $arrFuncional[4]
            . '.' . $arrFuncional[5]
            . '.' . $arrFuncional[6]
            . '.' . $arrFuncional[7];

        if ($tcrcod != TIPO_CREDITO_REMANEJAMENTO_PO) {
            $functionRemanejamentoSup = "verificaRemanejamentoPOSuplementacao(this.value, '$funcional');";
            $functionRemanejamentoCan = "verificaRemanejamentoPOCancelamento(this.value, '$funcional');";
        } else {
            $functionRemanejamento = "";
            $functionRemanejamentoCan = "";
        }
        ?>
                                                    <input
                                                        funcional="sup_<?php echo $funcional; ?>"
                                                        id="item_sup_<?php echo $chave; ?>"
                                                        type="text"
                                                        name="credito_sup[<?php echo $credito['dpaid']; ?>]"
                                                        value="<?php echo number_format($credito['sup'], 0, ',', '.'); ?>"
                                                        onkeyup="alterarSup( <?php echo $id; ?>, this ); <?php echo $functionRemanejamentoSup; ?>"
                                                        style="text-align:right;color:#000050;"
                                                        size="12"
                                                        class="normal"
        <?php echo $edicao_ativa ? '' : 'disabled="disabled"'; ?>
                                                        />
                                                </td>
                                                <td class="coluna12_credito" title="Cancelamento">
                                                    <input
                                                        funcional="can_<?php echo $funcional; ?>"
                                                        id="item_can_<?php echo $chave; ?>"
                                                        type="text"
                                                        name="credito_can[<?php echo $credito['dpaid']; ?>]"
                                                        value="<?php echo number_format($credito['can'], 0, ',', '.'); ?>"
                                                        onkeyup="alterarCan( <?php echo $id; ?>, this ); <?php echo $functionRemanejamentoCan; ?>"
                                                        style="text-align:right;color:#500000;"
                                                        size="12"
                                                        class="normal"
        <?php echo $edicao_ativa ? '' : 'disabled="disabled"'; ?>
                                                        />
                                                </td>
                                                <td class="coluna13_credito" title="Suplementação por Excesso">
                                                    <input
                                                        funcional="sup_ex_<?php echo $funcional; ?>"
                                                        id="item_vlexcesso_<?php echo $chave; ?>"
                                                        type="text"
                                                        name="credito_vlexcesso[<?php echo $credito['dpaid']; ?>]"
                                                        value="<?php echo number_format($credito['vlexcesso'], 0, ',', '.'); ?>"
                                                        onkeyup="alterarvlexcesso( <?php echo $id; ?>, this ); <?php echo $functionRemanejamentoSup; ?>"
                                                        style="text-align:right;color:#000050;"
                                                        size="12"
                                                        class="normal"
        <?php echo $edicao_ativa ? '' : 'disabled="disabled"'; ?>
                                                        />
                                                </td>
                                                <td class="coluna14_credito" title="Suplementação por Superávit">
                                                    <input
                                                        funcional="sup_s_<?php echo $funcional; ?>"
                                                        id="item_vlsuperavit_<?php echo $chave; ?>"
                                                        type="text"
                                                        name="credito_vlsuperavit[<?php echo $credito['dpaid']; ?>]"
                                                        value="<?php echo number_format($credito['vlsuperavit'], 0, ',', '.'); ?>"
                                                        onkeyup="alterarvlsuperavit( <?php echo $id; ?>, this ); <?php echo $functionRemanejamentoSup; ?>"
                                                        style="text-align:right;color:#000050;"
                                                        size="12"
                                                        class="normal"
        <?php echo $edicao_ativa ? '' : 'disabled="disabled"'; ?>
                                                        />
                                                </td>
                                                <td colspan="2" class="coluna15_credito" title="Saldo">
                                                    <span id="credito_saldo_<?php echo $id; ?>"></span>
                                                    <script type="text/javascript">
                                                        atualizarSaldoItem( <?php echo $id; ?>, <?php echo $credito['saldo'] ?> );
                                                    </script>
                                                </td>
                                            </tr>
        <?php $dot += $credito['dot']; ?>
        <?php $can += $credito['can']; ?>
        <?php $sup += $credito['sup']; ?>
        <?php $vlexcesso += $credito['vlexcesso']; ?>
        <?php $vlsuperavit += $credito['vlsuperavit']; ?>
        <?php $can_out += $credito['can_out']; ?>
        <?php $sup_out += $credito['sup_out']; ?>
        <?php $vlr_dot += $credito['vlrdot']; ?>

    <?php endforeach; ?>
    <?php //$saldo = ($dot + $vlexcesso + $vlsuperavit) - $can_out - $can + $sup_out + $sup;  ?>
    <?php $saldo = ($dot + $vlexcesso + $vlsuperavit) - $can + $vlr_dot + $sup; ?>
                                    </tbody>
    <?php if ($edicao_ativa) : ?>
                                        <tfoot style="color:#505050;">
                                            <tr bgcolor="#f0f0f0">
                                                <td nowrap="nowrap" class="coluna1_credito">
        <? $complemento = ' onkeypress="novoItem_moveProximoCampo( event, \'programa\' );" '; ?>
        <?php texto_popup('novo_prgcod', $sql_prgcod, 'Programa', 5, 3, '####', '', $complemento); ?>
                                                </td>
                                                <td nowrap="nowrap"  class="coluna2_credito">
        <? $complemento = ' onkeypress="novoItem_moveProximoCampo( event, \'acao\' );" onblur="filtraPO(this.value);"'; ?>
        <?php texto_popup('novo_acacod', $sql_acacod, 'Ação', 5, 3, '', '', $complemento); ?>
                                                </td>
                                                <td nowrap="nowrap"  class="coluna3_credito">
        <? $complemento = ' onkeypress="novoItem_moveProximoCampo( event, \'localizador\' );" '; ?>
        <?php texto_popup('novo_loccod', $sql_loccod, 'Localizador', 4, 3, '####', '', $complemento); ?>
                                                </td>
                                                <td nowrap="nowrap"  class="colunaPO_credito" id="tdPO"> &nbsp;

                                                    <!--
        <? $complemento = ' onkeypress="novoItem_moveProximoCampo( event, \'plocod\' );" '; ?>
        <?php texto_popup('novo_plocod', $sql_plocod, 'PO', 5, 3, '', '', $complemento); ?>
                                                    -->
                                                </td>
                                                <td nowrap="nowrap"  class="coluna4_credito">
        <? $complemento = ' onkeypress="novoItem_moveProximoCampo( event, \'natureza\' );" '; ?>
        <?php texto_popup('novo_ndpcod', $sql_natureza, 'Natureza de Despesa', 8, 7, '########', '', $complemento); ?>
                                                </td>
                                                <td nowrap="nowrap"  class="coluna5_credito">
        <? $complemento = ' onkeypress="novoItem_moveProximoCampo( event, \'fonte\' );" '; ?>
        <?php texto_popup('novo_foncod', $sql_fonte, 'Fonte de Recurso', 3, 2, '###', '', $complemento); ?>
                                                </td>
                                                <td nowrap="nowrap"  class="coluna6_credito">
        <? $complemento = ' onkeypress="novoItem_moveProximoCampo( event, \'idoc\' );" '; ?>
        <?php texto_popup('novo_idocod', $sql_idoc, 'IDOC', 4, 3, '####', '', $complemento); ?>
                                                </td>
                                                <td nowrap="nowrap"  class="coluna7_credito">
        <? $complemento = ' onkeypress="novoItem_moveProximoCampo( event, \'iduso\' );" '; ?>
        <?php texto_popup('novo_iducod', $sql_iduso, 'IDUSO', 1, 1, '#', '', $complemento); ?>
                                                </td>
                                                <td nowrap="nowrap"  class="colunaRP_credito">
        <? $complemento = ''; ?>
        <?php texto_popup('novo_irpcod', $sql_irpcod, 'RP', 1, 1, '#', '', $complemento); ?>
                                                </td>
                                                <td class="coluna8_credito">0</td>
                                                <td class="coluna9_credito"><span style="color:#000050;">0</span></td>
        <!--										<td class="coluna10_credito"><span style="color:#500000;">0</span></td>-->
                                                <td class="coluna11_credito">
                                                    <input
                                                        type="text"
                                                        name="novo_sup"
                                                        id="novo_sup"
                                                        value="0"
                                                        onkeyup="this.value=formatarNumero( pegarInteiro( this.value ) );"
                                                        onkeypress="novoItem_moveProximoCampo( event, 'sup' );"
                                                        style="text-align:right;color:#000050;"
                                                        size="12"
                                                        class="normal"
                                                        />
                                                </td>
                                                <td class="coluna12_credito">
                                                    <input
                                                        type="text"
                                                        name="novo_can"
                                                        id="novo_can"
                                                        value="0"
                                                        onkeyup="this.value=formatarNumero( pegarInteiro( this.value ) );"
                                                        onkeypress="novoItem_moveProximoCampo( event, 'can' );"
                                                        style="text-align:right;color:#000050;"
                                                        size="12"
                                                        class="normal"
                                                        />
                                                </td>
                                                <td class="coluna13_credito">
                                                    <input
                                                        type="text"
                                                        name="novo_vlexcesso"
                                                        id="novo_vlexcesso"
                                                        value="0"
                                                        onkeyup="this.value=formatarNumero( pegarInteiro( this.value ) );"
                                                        onkeypress="novoItem_moveProximoCampo( event, 'vlexcesso' );"
                                                        style="text-align:right;color:#000050;"
                                                        size="12"
                                                        class="normal"
                                                        />
                                                </td>
                                                <td class="coluna14_credito">
                                                    <input
                                                        type="text"
                                                        name="novo_vlsuperavit"
                                                        id="novo_vlsuperavit"
                                                        value="0"
                                                        onkeyup="this.value=formatarNumero( pegarInteiro( this.value ) );"
                                                        onkeypress="novoItem_moveProximoCampo( event, 'vlsuperavit' );"
                                                        style="text-align:right;color:#000050;"
                                                        size="12"
                                                        class="normal"
                                                        />
                                                </td>
                                                <td colspan="2"  class="coluna15_credito" nowrap="nowrap">
                                                    <input type="button" name="adicionar" value="Adicionar" onclick="novoItem_inserirDoFormulario();"/>
                                                </td>
                                            </tr>
                                        </tfoot>
    <?php endif; ?>
                                </table>
                            </div>
                        </td>
                    </tr>
                    <tr bgcolor="#dfdfdf">
                        <td class="coluna1_credito">&nbsp;</td>
                        <td class="coluna2_credito">&nbsp;</td>
                        <td class="coluna3_credito">&nbsp;</td>
                        <td class="colunaPO_credito">&nbsp;</td>
                        <td class="coluna4_credito">&nbsp;</td>
                        <td class="coluna5_credito">&nbsp;</td>
                        <td class="coluna6_credito" >&nbsp;</td>
                        <td class="coluna7_credito" >&nbsp;</td>
                        <td class="colunaRP_credito">&nbsp;</td>
                        <td align="right" class="coluna8_credito">
    <?php echo number_format($dot, 0, ',', '.'); ?>
                        </td>

                        <!--
                        <td align="right" class="coluna9_credito" style="color:#000050;">
    <?php echo number_format($sup_out, 0, ',', '.'); ?>
                        </td>
                        <td align="right" class="coluna10_credito" style="color:#500000;">
    <?php echo number_format($can_out, 0, ',', '.'); ?>
                        </td>
                        -->

                        <td align="right" class="coluna9_credito" style="color:#000050;">
    <?php echo number_format($vlr_dot, 0, ',', '.'); ?>
                        </td>

                        <td align="right" class="coluna11_credito" style="color:#000050;">
                            <span id="credito_sup">&nbsp;</span>
                        </td>
                        <td align="right" class="coluna12_credito" style="color:#500000;">
                            <span id="credito_can">&nbsp;</span>
                        </td>
                        <td align="right" class="coluna13_credito" style="color:#000050;">
                            <span id="credito_vlexcesso">&nbsp;</span>
                        </td>
                        <td align="right" class="coluna14_credito" style="color:#000050;">
                            <span id="credito_vlsuperavit">&nbsp;</span>
                        </td>
                        <td align="right" class="coluna15_credito">
                            <span id="credito_saldo">&nbsp;</span>
                        </td>
                        <td class="barra">&nbsp;</td>
                    </tr>
                </tbody>
            </table>
    <?php if ($edicao_ativa) : ?>
                <table align="center" border="0" cellspacing="0" bgcolor="#f5f5f5" cellpadding="2" style="color:101010;border-bottom:0px;" class="tabela">
                    <tr>
                        <td width="20%" align="right" class="SubTituloDireita">&nbsp;</td>
                        <td width="80%" align="left">
                            <input type="button" value="Gravar" name="gravar" onclick="enviarCreditos();"/>
                        </td>
                    </tr>
                </table>
    <?php endif; ?>
        </center>
    </form>
<?php } ?>


<script type="text/javascript">
	
	
    // ----- REALIZA OPERAÇÕES INICIAIS INICIAIS -----
	
    // calcula dotação inicial geral
    // calcula outros cancelamentos gerais
    // calcula outras suplementações gerais
    dot = <?php echo $dot; ?>;
    can_out = <?php echo $can_out; ?>;
    sup_out = <?php echo $sup_out; ?>;
    vlr_dot = <?php echo $vlr_dot; ?>;
	
    // captura os elementos para exibição de dados gerais
    // itens utilizados nas funções que detectam alterações de credito
    span_saldo = document.getElementById( 'credito_saldo' );
    span_can = document.getElementById( 'credito_can' );
    span_sup = document.getElementById( 'credito_sup' );
    span_vlexcesso = document.getElementById( 'credito_vlexcesso' );
    span_vlsuperavit = document.getElementById( 'credito_vlsuperavit' );
	
    var erro_item_extra = new Array();
<?php foreach ($creditos_extra as $extra) : ?>
            erro_item_extra['prg'] = <?php echo in_array('prg', $extra['erro']) ? 'true' : 'false'; ?>;
            erro_item_extra['aca'] = <?php echo in_array('aca', $extra['erro']) ? 'true' : 'false'; ?>;
            erro_item_extra['loc'] = <?php echo in_array('loc', $extra['erro']) ? 'true' : 'false'; ?>;
            erro_item_extra['plo'] = <?php echo in_array('plo', $extra['erro']) ? 'true' : 'false'; ?>;
            erro_item_extra['ndp'] = <?php echo in_array('ndp', $extra['erro']) ? 'true' : 'false'; ?>;
            erro_item_extra['fon'] = <?php echo in_array('fon', $extra['erro']) ? 'true' : 'false'; ?>;
            erro_item_extra['ido'] = <?php echo in_array('ido', $extra['erro']) ? 'true' : 'false'; ?>;
            erro_item_extra['idu'] = <?php echo in_array('idu', $extra['erro']) ? 'true' : 'false'; ?>;
            novoItem_inserir(
            '<?= $extra['prgcod'] ?>',
            '<?= $extra['acacod'] ?>',
            '<?= $extra['loccod'] ?>',
            '<?= $extra['ndpcod'] ?>',
            '<?= $extra['foncod'] ?>',
            '<?= $extra['idocod'] ?>',
            '<?= $extra['iducod'] ?>',
            '<?= number_format($extra['sup'], 0, ',', '.') ?>',
            '<?= number_format($extra['can'], 0, ',', '.') ?>',
            '<?= number_format($extra['vlexcesso'], 0, ',', '.') ?>',
            '<?= number_format($extra['vlsuperavit'], 0, ',', '.') ?>',
            erro_item_extra
        );
            erro_item_extra = new Array();
<?php endforeach; ?>
	
        atualizarSomas();
	
</script>