<?php
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$helperPendencia = new Par3_Helper_PendenciasEntidade();

require APPRAIZ.'includes/cabecalho.inc';
?>
<style>
.esconde{
    cursor:pointer;
}
.esconde:hover{
    background-color: #F5F5DC;
}
.processo_detalhe{
    color:blue;
    cursor:pointer;
}
.modal-dialog{
    margin-left: 0px;
    margin-right: 0px;
}
</style>
<script language="javascript" src="js/documentoLegado.js"></script>
<script>

</script>
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
        <div class="ibox float-e-margins">
            <div class="ibox-content">
                <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']?>"/>
                <?php echo $simec->title($controleUnidade->pegarNomeEntidade($_REQUEST['inuid'])); ?>
                <br>
                 
                <?php $controleUnidade->cabecalhoUnidade();?>
                <div class="ibox-title" tipo="PAR">
                    <h3>Pendências - Restrições e Informidades</h3>
                </div>
                <br />
                <?php $arrPendencias = $helperPendencia->recuperarPendenciasEntidade($_REQUEST['inuid']); ?>
                
                <?php $arrTiposPendencia = $helperPendencia->recuperarTiposPendencia(); ?>
                
                <?php if( count($arrPendencias) > 0 ){ ?>
                
		            <div class="row">   
		            
		            	<?php foreach ( $arrTiposPendencia as $tipoPendencia ) {?> 
		            	
		            		<?php $boExists = $helperPendencia->existePendenciaPorTipo($tipoPendencia, $_REQUEST['inuid']); ?>
		            		
		            		<?php $arAuxHelperTipoPendencia = $helperPendencia->controleTipoPendencia($tipoPendencia, $boExists); ?>
			                
			                <div class="col-lg-3">
			                
			                    <div class="widget style1 <?php echo $arAuxHelperTipoPendencia['widget'];?>-bg">
			                        
			                        <div class="row vertical-align">
			                            
			                            <div class="col-xs-9">
			                                
			                                <h3 class="font-bold"><?php echo $arAuxHelperTipoPendencia['description'];?></h3>
			                            
			                            </div>
			                            
			                            <div class="col-xs-3 text-right">
			                                
			                                <i class="fa <?php echo $arAuxHelperTipoPendencia['thumb'];?> fa-3x"></i>
			                            
			                            </div>
			                        </div>
			                    </div>
			                </div>
		                <?php } ?>
	                </div>
	                <div class="row">
	                
	                <?php foreach( $arrPendencias as $pendencia=>$pendenciasAux ){ ?>
	                
	                	<?php $arAuxHelperPendencia = $helperPendencia->controleTipoPendencia($pendencia); ?>       
							
						<div class="col-lg-6">
	                    	   
	                    	<div class=" widget <?php echo $arAuxHelperPendencia['widget']; ?>-bg  text-left" >
	                        
		                        <div class="slim-scroll">
		                            
		                            <i class="fa fa-exclamation-triangle fa-2x"></i>
		                            
		                            <span class="font-bold no-margins" style="font-size: 16pt;"> &nbsp; <?php echo $arAuxHelperPendencia['description']; ?> </span>		 
				                	
				                	<?php foreach( $pendenciasAux as $estado =>$tipoPendencia2 ){ ?>
		
				                		<?php foreach( $tipoPendencia2 as $municipio=>$tipoPendencia3 ){ ?>
				                		
				                			<?php $mun = new Par3_Model_Municipio(); ?>
				                			
				                		  	<h3> &nbsp; <?php echo $mun->descricaoMunicipio( $municipio) ; ?> </h3><br />
							                    
											<div class="list-group">
											 
					                        	<?php foreach( $tipoPendencia3 as $dados ){ ?>
					                               
							                            <a class="list-group-item active" href="#">
							                                
							                                <h4 class="list-group-item-heading"><?php echo $dados['predescricao'] ; ?> </h4>
							                                <p class="list-group-item-text"><?php echo $dados['situacao'] ; ?>  </p>
							                            </a>
					                         	<?php } ?>
				                         	</div>
					                    <?php } ?>
					                <?php } ?>
		                		</div>
				      		</div>
				   		</div>
	               <?php } ?> 
	               </div>
			<?php }else{ ?>
				<div class="row" style="padding-top: 20px;">
				
					<div class="row">   
		            
		            	<?php foreach ( $arrTiposPendencia as $tipoPendencia ) {?> 
		            	
		            		<?php $boExists = $helperPendencia->existePendenciaPorTipo($tipoPendencia, $_REQUEST['inuid']); ?>
		            		
		            		<?php $arAuxHelperTipoPendencia = $helperPendencia->controleTipoPendencia($tipoPendencia, $boExists); ?>
			                
			                <div class="col-lg-3">
			                
			                    <div class="widget style1 <?php echo $arAuxHelperTipoPendencia['widget'];?>-bg">
			                        
			                        <div class="row vertical-align">
			                            
			                            <div class="col-xs-9">
			                                
			                                <h3 class="font-bold"><?php echo $arAuxHelperTipoPendencia['description'];?></h3>
			                            
			                            </div>
			                            
			                            <div class="col-xs-3 text-right">
			                                
			                                <i class="fa <?php echo $arAuxHelperTipoPendencia['thumb'];?> fa-3x"></i>
			                            
			                            </div>
			                        </div>
			                    </div>
			                </div>
		                <?php } ?>
	                </div>
                	<div class="alert alert-success  text-center" >
                                                      
                    	<h4 class="font-bold no-margins"><i class="fa fa-check"></i>&nbsp; Não existem pendências para esta entidade. </h4>		 
		            </div>
			    </div>
			<?php } ?>  
            </div>
        </div>
    </div>
</div>
<div id="modal-form" class="modal fade" aria-hidden="true" style="width:100%">
    <center>
    	<div class="modal-dialog">
    		<div id="html_modal-form" class="ibox-content"></div>
    	</div>
    </center>
</div>
<script>
$(document).ready(function()
{

	$('.esconde').click(function()
	{
	    var id = $(this).attr('tipo');

	    if ($('#'+id).css('display') == 'none') {
	    	$('#'+id).show();
		} else {
	    	$('#'+id).hide();
		}

	});
});

</script>