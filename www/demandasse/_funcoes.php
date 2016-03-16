<?php

function alertlocation($dados) {
    die("<script>
			" . (($dados['alert']) ? "alert('" . $dados['alert'] . "');" : "") . "
			" . (($dados['location']) ? "window.location='" . $dados['location'] . "';" : "") . "
			" . (($dados['javascript']) ? $dados['javascript'] : "") . "
		 </script>");
}

function pegarDocidDemanda($dmdid) {
    global $db;
    $sql = "select docid from demandasse.demanda where dmdid = {$dmdid}";
    $docid = $db->pegaUm($sql);
    if (!$docid) {
        $docid = wf_cadastrarDocumento(WF_TPDID_DEMANDASSE_DEMANDA, "Demanda SE {$dmdid}");
        $db->executar("UPDATE demandasse.demanda SET docid = $docid where dmdid = {$dmdid}");
        $db->commit();
    }

    return $docid;
}

function formularioObservacoes($dmdid, $dmoid = '', $complementoDisabled = '') {
    $modelDemandaObservacao = new DemandaObservacao($dmoid);
    ?>
    <div class="well">
        <?php if (!$complementoDisabled){ ?>
        <form id="form-observacao" method="post" class="form-horizontal">
            <input name="dmoid" type="hidden" value="<?php echo $modelDemandaObservacao->dmoid; ?>" >
            <input name="dmdid" type="hidden" value="<?php echo $dmdid; ?>" >
            <input name="action" type="hidden" value="salvar_observacao" >
            <?php } ?>
            <fieldset>
                <legend>Observações</legend>
                <div class="form-group">
                    <label for="dmdtitulo" class="col-lg-2 control-label">Observação:</label>
                    <div class="col-lg-10">
                        <textarea class="form-control" rows="3" name="dmotexto" id="dmotexto"><?php echo $modelDemandaObservacao->dmotexto; ?></textarea>
                    </div>
                </div>
            </fieldset>
            <div>
                <button title="Salvar" id="btn-salvar-observacao" class="btn btn-success" type="button"><span class="glyphicon glyphicon-thumbs-up"></span> Salvar</button>
                <a title="Limpar"class="btn btn-warning" href=""><span class="glyphicon glyphicon-hand-left"></span> Limpar</a>
            </div>
            <?php if (!$complementoDisabled){ ?>
        </form>
    <?php } ?>
    </div>

    <script type="text/javascript">
        $(function(){
            $('#btn-salvar-observacao').click(function(){
                if(!$('#dmotexto').val()){
                    alert('Favor preencher o campo de texto.');
                    return false
                }

                options = {
                    success : function() {
                        jQuery("#div_listagem_observacao").load('/demandasse/demandasse.php?modulo=principal/demandasformulario&acao=A&dmdid='+$('#dmdid').val());
                    }
                }

                jQuery("#form-observacao").ajaxForm(options).submit();
            });
        });
    </script>
<?php
}
//
// * Imprime um conteúdo em formato Pdf, trocando os readers de resposta da requisição.
// * @param string $content Conteúdo para conversão em Pdf.
// */
function html2Pdf($content)
{
    // -- Preparando a requisição ao webservice de conversão de HTML para PDF do MEC.
    $content = http_build_query(
        array ('conteudoHtml' => utf8_encode($content))
    );

    $context = stream_context_create(
        array(
            'http' => array(
                'method' => 'POST',
                'content' => $content
            )
        )
    );

    // -- Fazendo a requisição de conversão
    $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);

    header('Content-Type: application/pdf');
    header("Content-Disposition: attachment; filename=demandas-SE.pdf");
    echo $contents;
    exit();
}

?>
