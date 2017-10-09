<?php
/**
 * Gestor do detalhamento do relatório do TCU.
 * @version $Id: relgestao.inc 94320 2015-02-23 21:09:34Z OrionMesquita $
 */

/**
 * Gerenciamento de mensagens.
 * @see Simec_Helper_FlashMessage
 */
include_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
$fm = new Simec_Helper_FlashMessage('planacomorc/relgestao');
/**
 * Funções do workflow.
 * @see workflow.php
 */
require_once APPRAIZ . 'includes/workflow.php';

/**
 * Criação de strings DML.
 * @see DB_Dml
 */
require APPRAIZ . 'includes/library/simec/DB/DML.php';

/**
 * Arquivo de funções da funcionalidade relatório do TCU.
 * @see _funcoestcu.php
 */
require(APPRAIZ . 'www/planacomorc/_funcoestcu.php');

// -- Modal de alteração de responsável
if ('carregarUsuarios' == $_POST['requisicao']) {
    require_once dirname(__FILE__) . '/popupresponsavel.inc';
    die();
}
// -- Salvando o responsável
if ('salvarResponsavel' == $_POST['requisicao']) {
    try {
        atribuirResponsavel($_POST['rldid'], $_POST['novo_usucpf']);
        $fm->addMensagem('Associação realizada com sucesso.');
    } catch (Exception $e) {
        $fm->addMensagem($e->getMessage(), Simec_Helper_FlashMessage::ERRO);
    }
    header('Location: ' . $_SERVER['HTTP_REFERER']);
    die();
}

if('apagar' == $_POST['requisicao']){
    try{
        deletarRelatorioGestao($_POST['rlgid']);
        $fm->addMensagem('Relatório excluído com sucesso.');
    } catch (Exception $ex) {
        $fm->addMensagem($ex->getMessage(), Simec_Helper_FlashMessage::ERRO);
    }
    header('Location: ' . $_SERVER['HTTP_REFERER']);
    die();
}

$rlgid = $_REQUEST['rlgid'];

// -- Perfis do usuário
$perfis = pegaPerfilGeral($_SESSION['usucpf']);
// -- Tipo de usuário
$usuarioTCU = in_array(PFL_RELATORIO_TCU, $perfis);

// -- Chamando o gerador de PDF
if (isset($_GET['pdf']) && true == $_GET['pdf']) {
    exportarRelatorio('pdf', $rlgid, $_GET['rldid']);
    die();
//-- Chamando o gerador de XLS
} else if(isset($_GET['xls']) && true == $_GET['xls']) {
    exportarRelatorio('xls', $rlgid, $_GET['rldid']);
	die();
}

$urlSelecaoAcao = 'planacomorc.php?modulo=principal/relatoriogestao/cadastro_gestao&acao=A';

if (isset($_POST['requisicao'])) {
    if ('loadPo' == $_POST['requisicao']) {
        $select = <<<DML
SELECT *
  FROM planacomorc.relgdados rld
  WHERE rld.rldid = %d
DML;
        $stmt = sprintf($select, $_POST['rldid']);
        if (!($dados = $db->pegaLinha($stmt))) {
            $dados = array();
        }
        die(simec_json_encode($dados));
    }

    if ('salvar' == $_POST['requisicao']) {
        $dadospost = $_POST['dados'];
        $dadospost['rldcod'] = strtoupper($dadospost['rldcod']);
        if (empty($dadospost['rldid'])) {
            // -- insert
            array_shift($dadospost);
            $dadospost['rlgid'] = $rlgid;
            $insert = 'INSERT INTO planacomorc.relgdados(' . implode(', ', array_keys($dadospost)) . ')'
                    . '  VALUES(:' . implode(', :', array_keys($dadospost)) . ')'
                    . '  RETURNING rldid';

            $dml = new Simec_DB_DML($insert);
            $dml->addParams($dadospost);

            // -- Executando o insert
            if ($rldid = $db->pegaUm($dml)) {
                $db->commit();
                $fm->addMensagem('Sua solicitação foi executada com sucesso.');

                incluirDocid('rldid', 'relgdados', $rldid, TPDID_RELATORIO_TCU, 'Relatório TCU');
            } else {
                $db->rollback();
                $fm->addMensagem('Não foi possível executar sua solicitação.', Simec_Helper_FlashMessage::ERRO);
            }
        } else {

            function sqlUpdate($carry, $item)
            {
                $carry[] = "{$item} = :{$item}";
                return $carry;
            }

            // -- update
            $update = 'UPDATE planacomorc.relgdados SET ' . implode(', ', array_reduce(array_keys($dadospost), 'sqlUpdate'))
                    . '  WHERE rldid = :rldid'
                    . '  RETURNING rldid';
            $dml = new Simec_DB_DML($update);
            $dml->addParams($dadospost);
            // -- Executando o update
            if ($rldid = $db->pegaUm($dml)) {
                $db->commit();
                $fm->addMensagem('Sua solicitação foi executada com sucesso.');
            } else {
                $db->rollback();
                $fm->addMensagem('Não foi possível executar sua solicitação.', Simec_Helper_FlashMessage::ERRO);
            }
        }

        $redirect = "planacomorc.php?modulo=principal/relatoriogestao/relgestao&acao=A&rlgid={$_REQUEST['rlgid']}";
        // -- Redirecionamento contextual
        if ('detalhamentopo' == $_REQUEST['aba']) {
            $redirect .= "&aba=po";
        }

        // -- redirect
        header("Location: {$redirect}");
        die();
    }

    if ('salvarQuestionario' == $_POST['requisicao']) {
    	if (trabalhaQuestionarioRelatorioGestao($_POST)) {
            $fm->addMensagem('Sua solicitação foi executada com sucesso.');
        }
        header('Location: ' . $_SERVER['HTTP_REFERER']);
        die();
    }
}

$sql = <<<DML
SELECT rgt.unicod,
       uni.unidsc,
       rgt.acacod,
       rgt.exercicio,
       rgt.acatipo
  FROM planacomorc.relgestao rgt
    INNER JOIN public.unidade uni USING(unicod)
  WHERE rlgid = %d
DML;
if (!($dadosdb = $db->pegaLinha(sprintf($sql, $rlgid)))) {
    $fm->addMensagem("Não foi encontrado um registro com o identificador '{$rlgid}'.", Simec_Helper_FlashMessage::ERRO);
    header("Location: {$urlSelecaoAcao}");
    die();
}

// -- Dados de identificação do relatório
$dadosacao['unicod'] = $dadosdb['unicod'];
$dadosacao['unidsc'] = $dadosdb['unidsc'];
$dadosacao['exercicio'] = $dadosdb['exercicio'];
$dadosacao['acacod'] = $dadosdb['acacod'];
$dadosacao['acatipo'] = $dadosdb['acatipo'];

switch ($_REQUEST['aba']) {
    case 'detalhamentopo':
    case 'justificativapo':
        $sql = <<<DML
SELECT rld.*,
       doc.esdid,
       usu.usunome,
       rpu.usucpf
  FROM planacomorc.relgdados rld
    LEFT JOIN planacomorc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
    LEFT JOIN seguranca.usuario usu ON (rpu.usucpf = usu.usucpf)
    LEFT JOIN workflow.documento doc USING(docid)
  WHERE rld.rlgid = %d
    AND rld.rldid = %d
DML;
        $sql = sprintf($sql, $rlgid, $_REQUEST['rldid']);
        break;
    case 'po':
        $sql = false;
        break;
    default:
        if ('N' == $dadosacao['acatipo']) {
            $sql = <<<DML
SELECT DISTINCT rld.rldid,
       rld.rlgid,
       COALESCE(rld.rldtipocod, aca.acatipodsc) AS rldtipocod,
       rlg.acacod AS codigo,
       rlg.unicod,
       COALESCE(rld.rldtitulo, aca.acatitulo) AS rldtitulo,
       COALESCE(rld.rldiniciativa, aca.acainiciativacod || ' - ' || aca.acainiciativadsc) AS rldiniciativa,
       COALESCE(rld.rldobjetivo, aca.acaobjetivodsc) AS rldobjetivo,
       COALESCE(rld.rldcodigoobjetivo, aca.acaobjetivocod) AS rldcodigoobjetivo,
       COALESCE(rld.rldprograma, aca.prgdsc) AS rldprograma,
       COALESCE(rld.rldcodigoprograma, aca.prgcod) AS rldcodigoprograma,
       COALESCE(rld.rldtipoprograma, aca.prgtipo) AS rldtipoprograma,
       rld.rldacaoprioritaria,
       rld.rldacaoprioritariatipo,
       rld.rlddotacaoinicial,
       rld.rlddotacaofinal,
       rlddespempenhada,
       rlddespliquidada,
       rlddesppaga,
       rldrapinscprocessado,
       rldrapinscnaoprocessado,
       COALESCE(rld.rldunidademedida, unm.unmdsc) AS rldunidademedida,
       rld.rlddescmeta,
       rldrapeaem0101,
       rldrapeavalorliquidado,
       rldrapeavalorcancelado,
       rldrapeadescricaometa,
       rldrapeaunidademedida,
       rldrapearealizado,
       rldmontanteprevisto,
       rldmontantereprogramado,
       rldmontanterealizado,
       rld.docid,
       doc.esdid,
       usu.usunome,
       rpu.usucpf
  FROM planacomorc.relgestao rlg
    LEFT JOIN planacomorc.relgdados rld ON(rlg.rlgid = rld.rlgid AND rld.rldtipo = 'acao')
    LEFT JOIN monitora.acao aca ON(rlg.acacod = aca.acacod AND rlg.unicod = aca.unicod AND rlg.exercicio = aca.prgano)
    LEFT JOIN public.unidademedida unm USING(unmcod)
    LEFT JOIN planacomorc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
    LEFT JOIN seguranca.usuario usu ON (rpu.usucpf = usu.usucpf)
    LEFT JOIN workflow.documento doc USING(docid)
  WHERE rlg.rlgid = %d
    AND aca.prgdsc is not null

DML;
        } else {
            $sql = <<<DML
SELECT rld.*,
       rlg.acacod AS codigo,
       rlg.unicod,
       usu.usunome,
       rpu.usucpf,
       doc.esdid
  FROM planacomorc.relgdados rld
    LEFT JOIN planacomorc.relgestao rlg USING(rlgid)
    LEFT JOIN planacomorc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
    LEFT JOIN seguranca.usuario usu ON (rpu.usucpf = usu.usucpf)
    LEFT JOIN workflow.documento doc USING(docid)
  WHERE rld.rlgid = %d
    AND rld.rldtipo = 'acaorap'
DML;
        }
        $sql = sprintf($sql, $rlgid);
        break;
}
$dados = array();
if ($sql && ($dadosdb = $db->pegaLinha($sql))) {
    $dados = $dadosdb;
}
unset($dadosdb, $sql);

// -- Configurações adicionais para os campos de input
$configHabil = array('habil' => 'N');
$configClasse = array('classe' => 'numerico');
$configMasc = array('masc' => '###.###.###.###.###') + $configClasse;

// -- Permissões de preenchimento de ações ou pos - responsabilidade
$permissoesRld = consultarPermissoes($rlgid);

/**
 * Cabeçalho padrão do sistema.
 * @See cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<style type="text/css">
.form-group img{display:none}
</style>
<script type="text/javascript" language="javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" language="javascript">
$(document).ready(function() {
    $('textarea').addClass('form-control');
    $('.radio_acaprioritaria').on("change", function () {
        var escolha = $(this).val();
        if ('t' === escolha) {
            $('#casoPositivo').show();
        } else {
            $('#casoPositivo').hide();
        }
        $('#rldacaoprioritaria').val(escolha);
    });
    $('.radio_acaprioritipo').on("change", function () {
        var escolha = $(this).val();
        $('#rldacaoprioritariatipo').val(escolha);
    });

    $('#relGestao').submit(function(){
        if ('t' === $('#rldacaoprioritaria').val()) {
            var acaprioritipo = $('#rldacaoprioritariatipo').val();

            if (('' == acaprioritipo) || ('undefined' === acaprioritipo)) {
                $('#modal-alert .modal-body').html("Ao definir uma ação como <u>prioritária</u>, você deve classificá-la no campo 'Caso Positivo'.");
                $('#modal-alert').modal();
                return false;
            }
        }
    });

    // -- Controlando os valores da ação prioritária
    var valAcaoPrioritaria = $('#rldacaoprioritaria').val();
    if ('t' == valAcaoPrioritaria) {
        $('#acaoprioritaria_t').click();

        var valAcaoPrioritariaTipo = $('#rldacaoprioritariatipo').val();
        if ('' != valAcaoPrioritariaTipo) {
            $('#acaprioritariatipo_'+valAcaoPrioritariaTipo.toLowerCase()).click();
        }
    }

    // -- Ativando a máscara dos campos numéricos
    $('.numerico').blur();

    // -- Gerando o PDF em outra janela
    $('#gerar-pdf').click(function(e){
        e.stopPropagation();
        window.open(window.location+'&pdf=true&rldid='+$('#rldid').val(), 'simec-pdf');
    });

	// -- Gerando o XLS em outra janela
    $('#gerar-xls').click(function(e){
        e.stopPropagation();
        window.open(window.location+'&xls=true&rldid='+$('#rldid').val(), 'simec-xls');
    });

    // -- Salvar responsável
    $('#modal-alterar-responsavel').on('click', '#salvar-responsavel', function(){
        $('#form-responsavel').submit();
    });

    // -- Carregar conteúdo da modal
    $('#mostar-modal-alterar-responsavel').click(function(){
        var url = 'planacomorc.php?modulo=principal/relatoriogestao/relgestao&acao=A&rlgid=3&aba=acao',
            params = {rldid: $(this).attr('data-rldid'), usucpf: $(this).attr('data-usucpf'), requisicao: 'carregarUsuarios'};
        $.post(url, params, function(response){
            $('#modal-alterar-responsavel .modal-body').html(response);
            $('#modal-alterar-responsavel').modal();
        });
    });
});
</script>
<br>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active"><a href="<?php echo $urlSelecaoAcao; ?>">Seleção de Ação</a></li>
        <li class="active">Dados da Ação</li>
    </ol>
    <div class="col-md-8 col-md-offset-2">
        <div class="panel panel-info">
            <div class="panel-heading">Informações da Ação</div>
            <table class="table">
                <tr>
                    <td style="width:25%"><strong>Unidade Orçamentária:</strong></td>
                    <td colspan="5"><?php echo "{$dadosacao['unicod']} - {$dadosacao['unidsc']}"; ?></td>
                </tr>
                <tr>
                    <td><strong>Ação:</strong></td>
                    <td><?php echo $dadosacao['acacod']; ?></td>
                    <td><strong>Tipo:</strong></td>
                    <td><?php echo formatarAcaTipo($dadosacao['acatipo']); ?></td>
                    <td><strong>Exercício:</strong></td>
                    <td><?php echo $dadosacao['exercicio']; ?></td>
                </tr>
            </table>
        </div>
    </div>
    <br style="clear:both" />
    <?php
    echo $fm->getMensagens();
    // -- URL base das abas
    $urlBaseDasAbas = '/planacomorc/planacomorc.php?modulo=principal/relatoriogestao/relgestao&acao=A&rlgid='.$rlgid.'&aba=';

    // -- Gerenciamento de abas
    $listaAbas = array();
    if ('N' == $dadosacao['acatipo']) {
        if (!$usuarioTCU) {
            $abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'acao');
        } else {
            $abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'justificativaacao');
        }
        if (!$usuarioTCU) {
            $listaAbas[] = array("id" => 1, "descricao" => "Ação", "link" => "{$urlBaseDasAbas}acao");
        }
        $listaAbas[] = array('id' => 2, 'descricao' => 'Justificativa da Ação', 'link' => "{$urlBaseDasAbas}justificativaacao");

        // -- Aba de POs só é exibida para usuário TCU, se ele tiver alguma permissão cadastrada
        if ((!$usuarioTCU) || ($usuarioTCU && in_array('po', $permissoesRld))) {
            $listaAbas[] = array("id" => 3, "descricao" => "Planos Orçamentários", "link" => "{$urlBaseDasAbas}po");
        }

        // -- Abas exibidas apenas qdo solicitadas
        if ('detalhamentopo' == $abaAtiva) {
            $complemento = "&rldid={$_REQUEST['rldid']}";
            $listaAbas[] = array(
                'id' => 4,
                'descricao' => 'Detalhamento Plano Orçamentário',
                'link' => "{$urlBaseDasAbas}detalhamentopo{$complemento}"
            );
        }
        if ('justificativapo' == $abaAtiva) {
            $complemento = "&rldid={$_REQUEST['rldid']}";
            $listaAbas[] = array(
                'id' => 5,
                'descricao' => 'Justificativa do Plano Orçamentário',
                'link' => "{$urlBaseDasAbas}justificativapo{$complemento}"
            );
        }
    } else { // -- 'R' == $dadosacao['acatipo']
        if (!$usuarioTCU) {
            $abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'acaorap');
        } else {
            $abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'justificativaacaorap');
        }
        if (!$usuarioTCU) {
            $listaAbas[] = array("id" => 1, "descricao" => "Ação RAP", "link" => "{$urlBaseDasAbas}acaorap");
        }
        $listaAbas[] = array('id' => 2, 'descricao' => 'Justificativa da Ação RAP', 'link' => "{$urlBaseDasAbas}justificativaacaorap");
    }

    echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}{$complemento}");

    /**
     * Incluíndo o arquivo de acordo com a aba selecionada.
     * Para cada aba, é chamado o arquivo início dentro do diretório
     * com o mesmo nome da aba selecionada.
     */
    require_once(dirname(__FILE__) . "/{$abaAtiva}/inicio.inc");
    ?>
</div>
<?php

/**
 * Popup de alteração de responsáveis.
 * @see popupresponsavel.inc
 */
require_once dirname(__FILE__) . '/popupresponsavel.inc';
