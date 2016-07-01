
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<?php
//error_reporting( E_ALL );

/*
  Sistema Simec
  Setor responsável: SPO-MEC
  Desenvolvedor: Equipe Consultores Simec
  Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
  Programador: Renan de Lima Barbosa (renandelima@gmail.com)
  Módulo: credito.inc
  Finalidade: remanejar créditos
 */

$boNovaDotacao = false;
if (isset($_SESSION['credito_mcrid'])) {

    $sql = "select mcrdotacaoautorizada from elabrev.momentocredito where mcrid = {$_SESSION['credito_mcrid']}";
    $rsDotacao = $db->pegaUm($sql);

    if ($rsDotacao == 't') {
        $boNovaDotacao = true;
    }
}

define('TIPO_CREDITO_REMANEJAMENTO_PO', '911');

if ($_REQUEST['acao'] == 'C') {
    unset($_SESSION['credito_mcrid']);
}

if (!$_SESSION['credito_mcrid']) {
    include( 'credito_escolher_momento.inc' );
} else {
    define('TIPO_PROPOSTA', 1);
    define('TIPO_PROJETO_LEI', 2);
    define('TIPO_LOA', 3);

    if ($_SESSION['creditos_adicionais_tcrid'] && $_SESSION['creditos_adicionais_unicod']) {
        $tcrid = $_SESSION['creditos_adicionais_tcrid'];
        $unicod = $_SESSION['creditos_adicionais_unicod'];
    }

    if ($_REQUEST['tcrid']) {
        $tcrid = (integer) $_REQUEST['tcrid'];
    }
    if ($_REQUEST['unicod']) {
        $unicod = $_REQUEST['unicod'];
    }
    $prgcod = $_REQUEST['prgcod'];
    $acacod = $_REQUEST['acacod'];
    $loccod = $_REQUEST['loccod'];

    //&acao=A&tcrid=208&unicod=26392&ungcod=
    //ajax begin
    if ($_REQUEST['acaoPO']) {

        $sql = "select tcrdsc, tcrcod from elabrev.tipocredito where tcrid = " . $tcrid;
        $dados = $db->recuperar($sql);
        $tcrcod = $dados['tcrcod'];

        $_SESSION['acaoPO'] = $_REQUEST['acaoPO'];

        if ($_REQUEST['acaoPO']) {

            $acaoWhere = " where a.acacod = '" . strtoupper($_REQUEST['acaoPO']) . "'";
        } else {
            $acaoWhere = " where 1 < 0";
        }

        $sql_plocod2 = "SELECT
                        po.planoorcamentario AS codigo, po.titulo AS descricao 
                    FROM 
                        elabrev.ws_planosorcamentariosdto AS po
                    JOIN 
                        elabrev.ws_acaodto AS a ON a.identificadorunico = po.identificadorunicoacao 
                    WHERE 
                        a.codigoacao = '" . strtoupper($_REQUEST['acaoPO']) . "'
                        AND a.codigoorgao = '" . $unicod . "'
                        AND po.planoorcamentario <> '0000'";
        //dbg($sql_plocod2,1);

        $complemento = ' onkeypress="novoItem_moveProximoCampo( event, \'plocod\' );" ';
        echo texto_popup('novo_plocod', $sql_plocod2, 'PO', 5, 3, '', '', $complemento);

        die;
    }
    //ajax end

    unset($_SESSION['creditos_adicionais_tcrid']);
    unset($_SESSION['creditos_adicionais_unicod']);

    // define se usuário deve ir a tela de escolha
    $escolher = $_REQUEST['escolher'] || !$tcrid || !$unicod;

    if ($boNovaDotacao) {
        require( 'credito_funcoes_dt.inc' );
    } else {
        require( 'credito_funcoes.inc' );
    }

    $ppoid = cre_pegar_proposta_ativa();

    if (!$ppoid) {
        include APPRAIZ . 'includes/cabecalho.inc';
        print '<br/>';
        monta_titulo($titulo_modulo, 'Suplementação e Cancelamento de Créditos Adicionais');
        ?>
        <br/>
        <p align="center">
            <span style="color:#aa0000">Atenção!</span>
            Não é possivel realizar nenhum operação para o exercício atual.
            <br/>
            A Lei Orçamentária Anual do ano <?php echo $_SESSION['exercicio']; ?> não existe.
        </p>
        <?php
    } else if ($escolher) {

        /*         * * Verifica se perfil do usuário é de Super usuário, CGO / Coordenador de Orçamento ou CGO / Equipe Orçamentária ** */
        $pflcods = array(SUPER_USUARIO, CGO_COORDENADOR_ORCAMENTO, CGO_EQUIPE_ORCAMENTARIA);
        if (!possuiPerfil($pflcods)) {
            $sql = "select u.unidataatualiza
                                    from public.unidade u 
                                    inner join elabrev.usuarioresponsabilidade ur ON ur.unicod = u.unicod
                                    where ur.rpustatus='A' and ur.usucpf = '" . $_SESSION['usucpf'] . "' limit 1";
            $unidAtualiza = $db->pegaUm($sql);

            if (!$unidAtualiza) {
                require( 'credito_unidade.inc' );
            } else {
                require( 'credito_escolher_filtro.inc' );
            }
        } else {
            // tela inicial
            require( 'credito_escolher_filtro.inc' );
        }
    } else {
        if ($boNovaDotacao) {
            require( 'credito_remanejar_dt.inc' );
        } else {
            require( 'credito_remanejar.inc' );
        }
    }
}
?>