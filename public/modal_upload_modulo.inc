<?php
$arquivoModulo = new Public_Model_ArquivoModulo();
if ($arquivoModulo->verificaPerfilUsuario()){?>
<div class="form-group">
    <div class="row">
        <label for="farqdescricao" class="col-lg-2 control-label">Titulo:</label>
        <div class="col-lg-10">
            <input type="text" value="" class="CampoEstilo normal form-control" placeholder="Insira o titulo do arquivo." title="Titulo" id="farqdescricao" name="farqdescricao" maxlength="255" size="2">
        </div>
    </div>
    <div class="row">
        <label for="farqmddescricao" class="col-lg-2 control-label" style="margin-top: 10px;">Descrição:</label>
        <div class="col-lg-10" style="margin-top: 10px;">
            <textarea class="CampoEstilo normal form-control" placeholder="Insira a descrição do arquivo." title="Descrição" id="farqmddescricao" name="farqmddescricao" rows="3" cols="255"></textarea>
        </div>                                
    </div>
</div>
    <form class="dropzone" method="POST" enctype="multipart/form-data" action="?modulo=principal/monitorar-pnc&acao=A&acoid=<?php echo $mAcompanhamento->acoid; ?>&aba=1&requisicao_upload=upload_arquivo" id="formularioAnexo" name="formularioAnexo">
        <input type="hidden" id="arqmdid" name="arqmdid">
        <input type="hidden" id="arqdescricao" name="arqdescricao">
        <input type="hidden" id="arqmddescricao" name="arqmddescricao">
        <div class="fallback">
            <input name="file" type="file" multiple />
        </div>
    </form>
<?php }?>    
<div id="listaArquivosModulo">
    <?php
    $arquivoModulo = new Public_Model_ArquivoModulo();
    $listaArquivos = $arquivoModulo->recuperaArquivosPorModulo();
    include_once APPRAIZ. "public/lista_arquivos_modulo.inc";
    ?>
</div>
<script>
    $(document).ready(function(){
        $("#farqdescricao").change(function(){
            $("#arqdescricao").val($("#farqdescricao").val());
        });
        $("#farqmddescricao").change(function(){
            $("#arqmddescricao").val($("#farqmddescricao").val());
        });        
        Dropzone.options.formularioAnexo = {
            init: function() {

                this.on("success", function(file, response){
                    var jsonResponse = $.parseJSON(response);
                    listarAnexosModulo();
                });

                this.on("queuecomplete", function(file){

                    // Armazena o objeto Dropzone para chamar métodos
                    objFormularioAnexo = this;
                    // Chama mensagem de sucesso
                    swal({
                      title: "",
                      text: "Arquivos salvos com sucesso!",
                      timer: 2000,
                      showConfirmButton: false,
                      type: "success"
                    }, function(){
                        // Fecha o swal alert
                        swal.close();
                        // limpa campo de upload
                        objFormularioAnexo.removeAllFiles();
                        // fecha modal após a seleção
                        $('#modal_upload').modal('hide');
                    });
                });
            }
        };         
    });

    function listarAnexosModulo(){
        $('#listaArquivosModulo').load('?modulo=principal/monitorar-pnc&acao=A&requisicao_upload=listar_arquivos_modulo');
    }

</script>    