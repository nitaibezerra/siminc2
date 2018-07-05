<form class="dropzone" method="POST" enctype="multipart/form-data" action="?modulo=principal/monitorar-pnc&acao=A&acoid=<?php echo $mAcompanhamento->acoid; ?>&aba=1&requisicao_upload=upload_arquivo" id="formularioAnexo" name="formularioAnexo">
    <input type="hidden" id="arqmdid" name="arqmdid">
    <div class="form-group">
        <div class="row">
            <label for="arqdescricao" class="col-lg-2 control-label">Titulo:</label>
            <div class="col-lg-10">
                <input type="text" value="" class="CampoEstilo normal form-control" placeholder="Insira o titulo do arquivo." title="Titulo" id="arqdescricao" name="arqdescricao" maxlength="255" size="2">
            </div>
        </div>
        <div class="row">
            <label for="arqdescricao" class="col-lg-2 control-label" style="margin-top: 10px;">Descrição:</label>
            <div class="col-lg-10" style="margin-top: 10px;">
                <textarea class="CampoEstilo normal form-control" placeholder="Insira a descrição do arquivo." title="Descrição" id="arqmddescricao" name="arqmddescricao" rows="3" cols="255"></textarea>
            </div>                                
        </div>
    </div>
    <div class="fallback">
        <input name="file" type="file" multiple />
    </div>
</form>
<div id="listaArquivosModulo">
    <?php
    $arquivoModulo = new Public_Model_ArquivoModulo();
    $listaArquivos = $arquivoModulo->recuperaArquivosPorModulo();
    include_once APPRAIZ. "public/lista_arquivos_modulo.inc";
    ?>
</div>