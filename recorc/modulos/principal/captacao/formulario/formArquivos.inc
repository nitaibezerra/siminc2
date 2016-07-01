<?php
/**
 * Formulário de upload de arquivos de alteração de previsão.
 * @version $Id: formArquivos.inc 98171 2015-06-03 14:22:44Z maykelbraz $
 */
?>
<script type="text/javascript" lang="JavaScript">
function editarArquivo(arqid, vieid)
{
    window.location.href = 'recorc.php?modulo=principal/captacao/alterarArquivo&acao=A&vieid='+<?php echo $_REQUEST['vieid']?>+'&arqid='+arqid;
}

function downloadArquivo(arqid, vieid)
{
    window.location.href = 'recorc.php?modulo=principal/captacao/formulario&acao=A&vieid='+<?php echo $_REQUEST['vieid']?>+'&download=S&arqid=' + arqid;
}

function deletarArquivo(arqid)
{
    if (confirm("Deseja excluir este arquivo?")) {
        var url = window.location;
        $.post(url + "&arqiddoc=" + arqid, function(html) {
            alert('Registro excluído com sucesso!');
            var url = window.location;
            window.location = url;
        });
    }
}
</script>
<div class="col-lg-12">
<?php
$sql = <<<DML
SELECT DISTINCT arq.arqid AS codarq,
                con.angdsc || '.' || arq.arqextensao AS arquivo,
                arq.arqtamanho,
                to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora AS arqdata,
                usu.usunome
  FROM recorc.anexogeral con
    INNER JOIN public.arquivo arq on con.arqid = arq.arqid
    INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
  WHERE con.angtip IS NULL
    AND capid = %d
DML;

$sql = sprintf($sql, $dadoscaptacao['capid']);
$cabecalho = array("Descrição", "Tamanho(bytes)", "Data inclusão", "Responsável");
echo '<br>';


$listagem = new Simec_Listagem();
$listagem->setFormOff();
$listagem->setCabecalho($cabecalho);
$listagem->setQuery($sql);

$listagem->addAcao('edit', 'editarArquivo')
    ->addAcao('download', 'downloadArquivo');

if (!in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
    // -- Apenas o Super Usuário ou a CGO podem apagar a Solicitação
    $listagem->addAcao('delete', 'deletarArquivo');
}
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
</div>
<div class="col-lg-12">
    <fieldset>
        <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal" enctype="multipart/form-data">
            <input type="hidden" name="acao" id="acao" value="incluirArquivo" />
            <input type="hidden" name="capid" value="<?php echo $dadoscaptacao['capid']; ?>" />

            <input type="file" name="file" id="file" class="btn btn-primary start" title="Selecionar arquivo" />
            <br style="clear:both" />
            <br />
            <br />
            <div id="botoesHabilitados">
            <?php if (podeSalvarCaptacao($dadoscaptacao, false)) { ?>
                <div class="form-group">
                    <button class="btn btn-primary" id="atualizar">
                        <span class="glyphicon glyphicon-ok"></span> Enviar arquivo
                    </button>
                </div>
            <?php } else {
                    gravacaoDesabilitada();
                  } ?>
            </div>
        </form>
    </fieldset>
</div>