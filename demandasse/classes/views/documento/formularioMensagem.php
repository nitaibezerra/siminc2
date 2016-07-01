<?
global $db;

$dmdid = $this->entity['dmdid']['value'];
$prcid_dest = $this->entity['prcid_dest']['value'];
if($prcid_dest){
	$ecpemailpara = $db->pegaUm("select prcremailesponsavel from demandasse.procedencia where prcstatus = 'A' and prcid = ".$prcid_dest);
}

?>

<div class="container" id="myModal">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h3 class="modal-title">
                    Enviar Mensagem
                    <!--                <small>na Instituição -->
                    <?php //echo $_SESSION['instituicao']['intdscrazaosocial'] ?><!--</small>-->
                </h3>
                <!--            <h4 class="modal-title"></h4>-->
            </div>
            <div class="modal-body">

               <div class="row">
				    <div class="col-md-12">
				        <div id="container_save">
				            <form id="form_save" method="post" class="form-horizontal" >
				            	
				                <div class="col-md-12">
				                    <div class="well">
				                    	
				                        <input name="controller" type="hidden" value="documento">
				                        <input name="action" type="hidden" value="salvarMensagem">
				                        <input id="dmdid" name="dmdid" type="hidden" value="<?=$dmdid?>">
				                        
				                        <div class="form-group">
			                                <label for="ecpemailpara" class="col-lg-2 control-label">Para:</label>
			                                <div class="col-lg-10">
			                                    <input id="ecpemailpara" name="ecpemailpara" type="text" class="form-control" required="required" disabled="disabled" value="<?=$ecpemailpara?>">
			                                </div>
				                        </div>
				                        
				                        <div class="form-group">
			                                <label for="ecpassunto" class="col-lg-2 control-label">Assunto:</label>
			                                <div class="col-lg-10">
			                                    <input id="ecpassunto" name="ecpassunto" type="text" class="form-control" required="required" disabled="disabled" value="Prazo atrasado do documento nº <?=$dmdid?>">
			                                </div>
				                        </div>
				                        
				                        <div class="form-group">
				                            <label for="ecpcorpoemail" class="col-lg-2 col-md-2 control-label" >Mensagem:</label>
				                            <div class="col-lg-10 col-md-10 ">
				                                <textarea  rows="10" id="ecpcorpoemail" name="ecpcorpoemail" class="form-control" placeholder="" required="required"></textarea>
				                            </div>
				                        </div>
				                        <div class="text-right">
				                            <button id="bt-enviar" type="button" class="btn btn-success">Enviar</button>
				                        </div>
				                    </div>
				                </div>
				            </form>
				        </div>
				
				        <div class="clearfix"></div>
				
				        <!--        --><?php //$modelDemanda->recuperarListagem(); ?>
				    </div>
				</div>
                

                <div class="col-lg-12">
                    <div class="page-header">
                        <h1 id="forms">
                            <!--                Dados da universidade --->
                            <small>
                                Mensagens Enviadas
                            </small>
                        </h1>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-12" id="container_listar_mensagem">
                        <?php $this->listarMensagemAction(); ?>
                    </div>
                </div>
            </div>
        <div class="modal-footer">
            <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
        </div>
    </div>
</div>
<script language="JavaScript">

    $('#bt-enviar').click(function () {
    	var emailto = "<?=$ecpemailpara?>";
    	if(emailto == ''){
    		alert("Favor preencher o campo destino na tela de documento!");
    	}else{
    	
        	//$('#form_save').saveAjax({clearForm: true, functionSucsess: 'fecharModal'});
        	
        	if($('#ecpcorpoemail').val() == ''){
        		alert("Preencha o campo Mensagem.");
        		return false;
        	}

        	//$('#form_save').submit();
        	
        	$('#ecpemailpara').removeAttr('disabled');
        	$('#ecpassunto').removeAttr('disabled');
        	
        	$('#form_save').isValid(function(isValid){
	            if(isValid){
					//$('#form_save').saveAjax({clearForm: true, functionSucsess: 'listarArquivos'});
	                $('#form_save').ajaxSubmit(function(){
	                    //carregarFormulario();
	                    
	                    $('#myModal').hide();
	                    
	                    html = '<div class="col-lg-12"><div class="alert alert-dismissable alert-success"><strong>Mensagem Enviada com Sucesso!</strong><a class="alert-link" href="#"></a></div></div>';
	                    $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
	                    
	                });
	            }
        	});
        }
    });

</script>