<div class="ibox-content">
    <div class="form-group">
        <div class="col-lg-12">
            <table width="80%" cellspacing="0" class="table table-striped table-hover table-bordered">
                <thead>
                    <tr class="text-center">
                        <th style="text-align: center;">Açoes</th>
                        <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Data</th>
                        <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Arquivo</th>
                        <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Titulo</th>
                        <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Descriçao</th>
                        <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Responsavel</th>
                    </tr>
                </thead>
                <tbody>
                    <?php if($listaArquivos): ?>
                        <tbody>
                        <?php foreach($listaArquivos as $arquivo): ?>
                            <tr>
                                <td>
                                    <span class="glyphicon glyphicon-trash btnRemoverAnexosModulos link" title="Excluir o arquivo <?php echo $arquivo['arqnome']. '.'. $arquivo['arqextensao']; ?>" data-arqmdid="<?php echo $arquivo['arqmdid']; ?>" />
                                </td>
                                <td style="border: 1px #e5d9c5 solid;">
                                    <?php echo $arquivo['data']; ?>
                                </td>
                                <td style="border: 1px #e5d9c5 solid;">
                                    <a href="javascript:pegarArquivo(<?php echo $arquivo['arqid']; ?>);"><?php echo $arquivo['arqnome']; ?></a>
                                </td>
                                <td style="border: 1px #e5d9c5 solid;">
                                    <?php echo $arquivo['arqdescricao']; ?>
                                </td>
                                <td style="border: 1px #e5d9c5 solid;">
                                    <?php echo $arquivo['arqmddescricao']; ?>
                                </td>                                
                                <td style="border: 1px #e5d9c5 solid;">
                                    <?php echo $arquivo['usunome']; ?>
                                </td>
                            </tr>
                        <?php endforeach; ?>
                        </tbody>
                    <?php endif; ?>
                </tbody>
            </table>
        </div>
    </div>
</div>
<script>
$(document).ready(function(){
    $(".btnRemoverAnexosModulos").click(function(){
        arqmdid=$(this).data('arqmdid');
        $.post('?modulo=principal/monitorar-pnc&acao=A&requisicao_upload=remover_arquivos_modulo&arqmdid='+arqmdid,
            function(){
                listarAnexosModulo();
            }
        );
    });
});
function pegarArquivo(arqid){
    window.open('?modulo=principal/monitorar-pnc&acao=A&requisicao_upload=download_arquivos_modulo&arqid='+arqid);    
}
</script>
