<?php
/**
 * Upload de arquivo de previsão de captação / receita
 * $Id: previsaoupload.inc 98070 2015-06-02 13:33:53Z werteralmeida $
 */

ini_set("memory_limit", "2048M");
set_time_limit(300000);

/**
 *
 * @global cls_banco $db
 * @param type $dados
 * @return string
 */
function upsertPrevisao($dados)
{
    global $db;
    $update = <<<DML
UPDATE recorc.previsaoreceita
  SET valor = (SELECT valor
                 FROM recorc.dadoscargaprevisao dcp
                 WHERE dcp.dcpid = %d)
  WHERE EXISTS (SELECT 1
                  FROM recorc.dadoscargaprevisao dcp
                  WHERE dcp.dcpid = %d
                    AND previsaoreceita.exercicio = dcp.exercicio
                    AND previsaoreceita.unicod = dcp.unicod
                    AND previsaoreceita.foncod = dcp.foncod
                    AND previsaoreceita.nrccod = dcp.nrccod
                    AND previsaoreceita.mes = dcp.mes)
    AND prfid = {$dados['prfid']}
  RETURNING prrid
DML;

    $insert = <<<DML
INSERT INTO recorc.previsaoreceita(valor, exercicio, unicod, foncod, nrccod, mes, prfid)
SELECT dcp.valor, dcp.exercicio, dcp.unicod, dcp.foncod, dcp.nrccod, dcp.mes, {$dados['prfid']}
  FROM recorc.dadoscargaprevisao dcp
  WHERE dcp.dcpid = %d
DML;

    $insertVinculacao = <<<DML
INSERT INTO recorc.vinculacaoexercicio (exercicio, unicod, foncod, nrccod, prfid)
SELECT dcp.exercicio, dcp.unicod, dcp.foncod, dcp.nrccod, {$dados['prfid']}
  FROM recorc.dadoscargaprevisao dcp
  WHERE dcp.dcpid = %d
    AND NOT EXISTS (SELECT 1
                      FROM recorc.vinculacaoexercicio vex
                      WHERE dcp.dcpid = %d
                        AND dcp.exercicio = vex.exercicio
                        AND dcp.unicod = vex.unicod
                        AND dcp.foncod = vex.foncod
                        AND dcp.nrccod = vex.nrccod
                        AND vex.prfid = {$dados['prfid']})
DML;

    $qtdAtualizados = $qtdInseridos = $qtdVincInseridas = 0;

    // -- Carregando as naturezas para validação de naturezas não cadastradas
    $sqlNatureza = <<<DML
SELECT DISTINCT nrccod AS cod
  FROM public.naturezareceita
DML;
    $naturezas = $db->carregar($sqlNatureza);
    array_walk($naturezas, 'multipleArrayToArray');

    // -- Carregando as fontes para validação de fontes não cadastradas
    $sqlFontes = <<<DML
SELECT DISTINCT foncod AS cod
  FROM public.fonterecurso
DML;
    $fontes = $db->carregar($sqlFontes);
    array_walk($fontes, 'multipleArrayToArray');

    $sql = <<<DML
SELECT dcpid, nrccod, foncod
  FROM recorc.dadoscargaprevisao
DML;

    $naturezaNaoCadastrada = $fonteNaoCadastrada = array();

    foreach ($db->carregar($sql) as $dados) {
        $dcpid = $dados['dcpid'];
        $nrccod = $dados['nrccod'];
        $foncod = $dados['foncod'];

        $pula = false;
        // -- Natureza cadastrada no banco?
        if (!in_array($nrccod, $naturezas)) {
            if (!in_array($nrccod, $naturezaNaoCadastrada)) {
                $naturezaNaoCadastrada[] = $nrccod;
            }
            $pula = true;
        }
        // -- Fonte cadastrada no banco?
        if (!in_array($foncod, $fontes)) {
            if (!in_array($foncod, $fonteNaoCadastrada)) {
                $fonteNaoCadastrada[] = $foncod;
            }
            $pula = true;
        }
        if ($pula) {
            continue;
        }

        $stmt = sprintf($update, $dcpid, $dcpid);
        $atualizado = $db->carregar($stmt);

        if (empty($atualizado)) {
            $stmt = sprintf($insert, $dcpid);
            $db->executar($stmt);
            $qtdInseridos++;
        } else {
            $qtdAtualizados++;
        }

        // -- Inserindo vinculação de exercicio
        $stmt = sprintf($insertVinculacao, $dcpid, $dcpid);
        $qtdVincInseridas += pg_affected_rows($db->executar($stmt));
    }

    $result = array('sucesso' => false, 'msg' => 'Não foi possível inserir as informações submetidas.');
    if ($db->commit()) {
        $result['sucesso'] = true;
        $result['msg'] = "Foi(ram) <strong>adicionado(s) {$qtdInseridos}</strong> novo(s) registro(s)."
                       . "<br /><br /><strong>{$qtdAtualizados} registro(s)</strong> foi(ram) <strong>atualizados(s)</strong>."
                       . "<br /><br /><strong>{$qtdVincInseridas}</strong> novas vinculações foram criadas.";
        if (!empty($naturezaNaoCadastrada) || !empty($fonteNaoCadastrada)) {
            $result['msg'] .= '<br /><br /><strong>Importante!</strong>';
        }
        if (!empty($naturezaNaoCadastrada)) {
            $result['msg'] .= '<br /><br />Naturezas não cadastradas na base:<br />' . implode('<br />', $naturezaNaoCadastrada);
        }
        if (!empty($fonteNaoCadastrada)) {
            $result['msg'] = '<br /><br />.Fontes não cadastradas na base:<br /> ' . implode('<br />', $fonteNaoCadastrada);
        }
    }
    return $result;
}

function multipleArrayToArray(&$item)
{
    $item = $item['cod'];
}

function explodeCSVLine($line)
{
    return explode(';', trim($line));
}

if (isset($_POST['action'])) {
    switch ($_POST['action']) {
        case 'carregar':
            $errors = array(
                UPLOAD_ERR_OK => 'Arquivo carregado com sucesso.',
                UPLOAD_ERR_INI_SIZE => 'O tamanho do arquivo é maior que o permitido.',
                UPLOAD_ERR_PARTIAL => 'Ocorreu um problema durante a transferência do arquivo.',
                UPLOAD_ERR_NO_FILE => 'O arquivo enviado estava vazio.',
                UPLOAD_ERR_NO_TMP_DIR => 'O servidor não pode processar o arquivo.',
                UPLOAD_ERR_CANT_WRITE => 'O servidor não pode processar o arquivo.',
                UPLOAD_ERR_EXTENSION => 'O arquivo recebido não é um arquivo válido.'
            );
            // -- Processando o arquivo
            if (isset($_FILES['previsao']['tmp_name']) && is_file($_FILES['previsao']['tmp_name'])) {
                $data = file($_FILES['previsao']['tmp_name']);
                // -- Eliminando cabeçalho
                array_shift($data);
                // -- Quebrando as linhas em colunas
                $data = array_map(explodeCSVLine, $data);

                global $db;
                //    $sqlTruncate = "truncate recorc.dadoscargaprevisao";
                $sqlTruncate = "DELETE FROM recorc.dadoscargaprevisao";
                $db->executar($sqlTruncate);
                $db->commit();

                $exercicioValido = true;
                $sql = 'INSERT INTO recorc.dadoscargaprevisao (exercicio, unicod, foncod, nrccod, mes, valor) values';
                foreach ($data as $campos) {
                    if ($campos[0] != $_SESSION['exercicio']) {
                        $exercicioValido = false;
                        break;
                    }
                    $campos[5] = str_replace(',', '.', $campos[5]);
                    $sql .= "('$campos[0]', '$campos[1]','$campos[2]', '$campos[3]', '$campos[4]', $campos[5]),";
                }

                // -- Apenas se todos os exercicios estiverem válidos, prossegue com a inclusão
                if ($exercicioValido) {
                    $sqlF = substr($sql, 0, -1);
                    $db->executar($sqlF);
                    $db->commit();

                    $sqlDadosBanco = <<<DML
SELECT exercicio,
       dac.unicod || ' - ' || unidsc AS unidade,
       dac.foncod || ' - ' || COALESCE(fon.fondsc, '?') AS fonte,
       dac.nrccod || ' - ' || COALESCE(nrc.nrcdsc, '?') AS natureza,
       mes,
       valor
  FROM recorc.dadoscargaprevisao dac
    LEFT JOIN public.unidade uni ON (uni.unicod = dac.unicod)
    LEFT JOIN public.fonterecurso fon ON (fon.foncod = dac.foncod)
    LEFT JOIN public.naturezareceita nrc ON (nrc.nrccod = dac.nrccod)
  ORDER BY uni.unicod,
           fon.foncod,
           nrcid
DML;
                    $qtdRegistros = $db->pegaUm("SELECT COUNT(1) FROM({$sqlDadosBanco}) data");
                    setFlashMessage(
                        (UPLOAD_ERR_OK == $_FILES['previsao']['error']),
                        $errors[$_FILES['previsao']['error']]
                    );
                } else {
                    // -- error
                    setFlashMessage(
                        false,
                        "O arquivo carregado apresenta dados que não correspondem ao exercício selecionado."
                    );
                }
            }
            break;
        case 'importar':
            setFlashMessageArray(upsertPrevisao($_POST['dados']));
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
            break;
    }
}

/**
 * Cabeçalho do SIMEC
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script type="text/javascript" src="../library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
<link rel="stylesheet" type="text/css" href="../library/bootstrap-switch/stylesheets/bootstrap-switch.css">
<style>
.pad-12{padding-top:12px!important}
</style>
<script type="text/javascript" language="JavaScript">
$(document).ready(function(){
    $('input[type=file]').bootstrapFileInput();
    $('#buttonSend').click(function(){
        $('#previsaoupload').submit();
    });
    $('#import-data').click(function(){
        $('.has-error').removeClass('has-error');
        if ('' == $('#prfid').val()) {
            var label = $('#label').text();
            $('#prfid_group').addClass('has-error');
            var msg = '<div class="alert alert-danger text-center">' + '<p>O campo <strong>' + label.replace(':', '') + '</strong> é obrigatório e não pode ser deixado em branco.</p>' + '</div>';
            $('.modal-body').html(msg);
            $('#modal-alert').modal();
            return false;
        }
        $('#dados_prfid').val($('#prfid').val());
        $('#listagem_action').val('importar');

        // -- na segunda etapa, executar a validação de período de referência
        $('.form-listagem').submit();
    });
});
</script>
<div class="row">
    <div class="col-md-12">
<!--        <div class="page-header">
            <h4>Carga da Previsão de Receita</h4>
        </div><br />-->
        <ol class="breadcrumb">
            <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li>Parâmetros SPO</li>
            <li class="active">Carga da Previsão de Receita</li>
        </ol>
        <div class="well">
            <form name="previsaoupload" id="previsaoupload" enctype="multipart/form-data" method="POST">
                
                <div class="alert alert-warning" role="alert"><b>AVISO:</b> O ano na coluna exercicio do arquivo deve ser semrpe o Ano Corrente, independente se os dados são para a PLOA do próximo ano.</div>
                
                <?php if (isset($_FILES['previsao']) && (UPLOAD_ERR_OK == $_FILES['previsao']['error']) && $exercicioValido): ?>
				<div class="form-group form-horizontal" id="prfid_group">
					<label for="inputExercicio" id="label" class="col-lg-2 control-label pad-12">Período de referência:</label>
					<div class="col-lg-7">
                        <?php
                        $sqlPeriodo = <<<DML
SELECT prfid AS codigo,
       prfdsc AS descricao
  FROM recorc.periodoreferencia
  WHERE exercicio = '{$_SESSION['exercicio']}'
    AND prfstatus = 'A'
  ORDER BY prfid DESC
DML;
                        $db->monta_combo('dados[prfid]', $sqlPeriodo, 'S', 'Selecione um período', null, null, null, null, 'N', 'prfid', null, '', null, 'class="form-control chosen-select" style="width=100%;" required="required"');
                        ?>
                    </div>
				</div>
                <br style="clear:both" />
                <?php endif; ?>
                <div class="form-group">
                    <div class="col-md-10">
                        <input type="file" title="Carregar arquivo de previsão (.csv)"
                               name="previsao" class="btn btn-primary start" />
                        <input type="hidden" name="action" id="action" value="carregar" />
                    </div>
                    <div class="col-md-2">
                        <div class="alert alert-info" style="margin-bottom:0">
                            <p>
                                <i class="glyphicon glyphicon-download-alt"></i>
                                <a href="arquivos/previsao.csv" target="_blank">Download do modelo</a>.
                            </p>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-lg-10">
                            <button class="btn btn-success" id="buttonSend" type="submit"><i class="glyphicon glyphicon-upload"></i> Enviar</button>
                            <button class="btn btn-warning" id="buttonClear" type="reset">Limpar</button>
                        </div>
                    </div>
                    <div style="clear:both"></div>
                </div>
            </form>
        </div>
<?php
echo getFlashMessage();
if ($qtdRegistros > 120) {
    $sqlDadosBanco .= ' LIMIT 120';
?>
        <div class="alert alert-info col-md-offset-3 col-md-6 text-center">
            <i class="glyphicon glyphicon-exclamation-sign"></i> O arquivo carregado é muito grande. Exibindo apenas
            120 linhas do total de <?php echo $qtdRegistros; ?>.
        </div>
<?php
}
if (isset($_FILES['previsao']) && (UPLOAD_ERR_OK == $_FILES['previsao']['error']) && $exercicioValido) {
    require(dirname(__FILE__) . '/previsaolistagem.inc');
}
?>
    </div>
</div>
