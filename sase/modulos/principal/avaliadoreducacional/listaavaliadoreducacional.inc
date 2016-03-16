<?php

	$colTitulo = 'col-sm-4';
	$colCampo  = 'col-sm-4';
	if ($_POST['formulario']) {
		// Monta um array com os dados enviados na requisição		
			switch ($_POST['acao']){
				case 'deleta':
					$res = $avaliadoreducacional->excluir($_POST['aveid']);
					
					if ($res){
						echo "
									<script>
										alert('Avaliador Educacional inativado com sucesso.');
									</script>";
					} else {
						echo "
									<script>
										alert('Falha ao inativar os dados do avaliador.');
									</script>";
					}
					$avaliadoreducacional = new AvaliadorEducacional();
					break;
							
				case 'pesquisa':
					$_REQUEST['avenumcpf'] = str_replace('-','',str_replace('.','', $_REQUEST['avenumcpf']));
					$_REQUEST['avecep'] = str_replace('-', '', $_REQUEST['avecep']);
					$avaliadoreducacional->popularDadosObjeto();
					break;			

				case 'exporta_xls':
					$avaliadoreducacional->recuperarListagem('xls');
					exit();
					
				case 'gera_ficha':
					ob_clean ();

					$html = $avaliadoreducacional->fichaCadastral($_REQUEST['aveid']);
					
					$content = http_build_query(array(
							'conteudoHtml' => utf8_encode($html)
					));
					
					$context = stream_context_create(array(
							'http' => array (
									'method' => 'POST',
									'content' => $content
							)
					));
					
					$contents = file_get_contents ( 'http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
					header ( 'Content-Type: application/pdf' );
					header ( 'Content-Disposition: attachment; filename=Ficha_Cadastral_'.$_GET['ted']);
					echo $contents;						
					exit();
		}
	}
?>

<style>
	.titulo-grupo {
		font-weight: bold;
		margin-bottom: 5px;
	}
	
</style>

<script language="javascript" type="text/javascript">

	function carregaMunicipioPelaUf(seluf, id) {
	    divCarregando();
	    jQuery.ajax({
	        type: 'POST',
	        url: 'sase.php?modulo=principal/avaliadoreducacional&acao=A',
	        data: {requisicao: 'carrega_municipio', seluf: seluf, id: id},
	        async: false,
	        success: function(data) {
		        console.log(1);
	            $('#divMunicipio').html(data);
	            console.log(data);
	            divCarregado();
	        }
	    });
	}
		
	function showTipoDeficiencia() {
		if ( $('#avedeficiencia' ).val() == 't'){
			$('#divTipoDeficiencia').removeAttr('style');
		} else {
			$('#divTipoDeficiencia').attr('style','display: none;');
		}
	}
	
</script>

<!-- html -->
<div id="container">

<div class="row">
<div class="row col-md-12">


<form id="form-save" method="post" name="formCadastroLista" role="form" class="form-horizontal">
<input type="hidden" id="requisicao" name="requisicao" value="" />
<input name="formulario" value="1" type="hidden"/>
<input name="acao" value="pesquisa" type="hidden"/>
<input name="aveid" value="<?=@$_GET['aveid']?>" type="hidden"/>
<input name="usucpf" value="<?=@$_SESSION['usucpf']?>" type="hidden"/>
    <input type="hidden" name="avediretoria" id="avediretoria" value="<?=@$avediretoria?>"/>
				
<div class="row col-md-12">
<div class="well">
<fieldset>
<legend>Lista de Avaliadores Educacionais</legend>

	<div class="form-group tipo-judicial">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label">Nome:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avenome" name="avenome" type="text"
	               class="form-control" placeholder="" value="<?php echo $avaliadoreducacional->arAtributos['avenome'] ?>">
	    </div>
	</div>
	<div class="form-group tipo-judicial">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label">CPF:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avenumcpf" name="avenumcpf" type="text"
	               class="campoCpf form-control" placeholder="" value="<?php echo $avaliadoreducacional->arAtributos['avenumcpf'] ?>">
	    </div>
	</div>
            
	<div class="form-group">
	    <label for="dmdtipo" class="col-lg-4 col-md-4 control-label">Perfil:</label>
	
	    <div class="col-lg-8 col-md-8 ">
            <?php echo $avaliadoreducacional->getTipos() ?>
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdtipo" class="col-lg-4 col-md-4 control-label">UF:</label>
	
	    <div class="col-lg-8 col-md-8 ">
            <?php echo $avaliadoreducacional->getUfs('estuf') ?>
	    </div>
	</div>
	
</fieldset>
</div>
</div>

	<div class="text-right">
		<button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="pesquisarAvaliadorEducacional()"><span
			class="glyphicon glyphicon-search"></span> Pequisar
		</button>
		<button title="Salvar" class="btn btn-primary" type="button" id="btnPesquisar" onclick="exportaXls()">
			Exportar XLS
		</button>
		</div>
	<br/>

</form>
</div>
</div>
<?php $avaliadoreducacional->recuperarListagem(); ?>
</div>

<div class="modal fade" id="insert-new" tabindex="-1" role="dialog" aria-labelledby="ncModalLabel" aria-hidden="true">
    <div class="modal-dialog" style="width:70%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">
                    <span aria-hidden="true">&times;</span>
                    <span class="sr-only">Close</span>
                </button>
                <h4 class="modal-title" id="modal-title">Responsabilidades</h4>
            </div>
            <div class="modal-body" id="modal-body">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>


</div>
<!-- html / -->

<!-- script js -->
<script>

	/**
	* 	Funções executadas quando a página é carregada
	*/
	$(document).ready(function(){
		$('.campoCpf').mask("999.999.999-99");
		$('.campoDdd').mask("99");
		$('.campoTel').mask("(99) 9999-99999");
		if ($('.campoTel').val() == '() '){ // Limpa os campos de telefone
			$('.campoTel').val('')
		}
        $('.campoData').mask('99/99/9999');
        $('.campoData').datepicker();
});
								
	/**
	*	Clona o formulário de dados profissionais
	*/	
	function clonaDadosProfissionais(){
		var con = $("#hidCont").val();
		con++;
		$("#divDados").clone().children().each(function() {
			$(this).find('#aveorgao').attr("id", "aveorgao" + con).attr("name", "aveorgao" + con);
			$(this).find('#avesetor').attr("id", "avesetor" + con).attr("name", "avesetor" + con);
			$(this).find('#avecargo').attr("id", "avecargo" + con).attr("name", "avecargo" + con);
			$(this).find('#avefuncao').attr("id", "avefuncao" + con).attr("name", "avefuncao" + con);
			$(this).find('#btnAdd').attr("style", "display: none");
		}).appendTo('#divDadosProfissionais');
		$("#hidCont").val(con);
	}

	function pesquisarAvaliadorEducacional(){
		jQuery('[name=acao]').val('pesquisa');
		jQuery('[name=formCadastroLista]').submit();	
	}
	
	function listaAvaliadorEducacional(aveid) {
		window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria=<?=@$avediretoria?>&acao=A&aba=cadastro&aveid='+aveid;
	} 

	function deleteAvaliadorEducacional(id) {
		if (confirm('Deseja inativar este Avaliador Educacional?')){
			jQuery('[name=acao]').val('deleta');
			jQuery('[name=aveid]').val(id);
			jQuery('[name=formCadastroLista]').submit();	
		}
	}

	function exportaXls() {
		jQuery('[name=acao]').val('exporta_xls');
		jQuery('[name=formCadastroLista]').submit();	
	}

	function geraFichaCadastral(aveid){
		jQuery('[name=acao]').val('gera_ficha');
		jQuery('[name=aveid]').val(aveid);
		jQuery('[name=formCadastroLista]').submit();	
	}
	
	function showresponsabilidade(id, usucpf, taeid, cpf, nome){
		//alert(aveid + ' ' + taeid);
	    $.ajax({
	        url:'sase.php?modulo=principal/avaliadoreducacional&avediretoria=<?=@$avediretoria?>&acao=A',
	        data: {requisicao:'modalResponsabilidade',usucpf: usucpf, taeid: taeid},
	        success: function(data) {
	            console.log(data);
	            $("#modal-body").html(data);
	            $("#modal-title").html('Responsabilidade ('+cpf+' - '+nome+')');
	            $("#insert-new").modal("show");
	        }
	    })
	}
	
	/**
	 * Envia Formulário
	 */
	 function enviaFormulario(){
 		jQuery('[name=formCadastroLista]').submit();
	 }


</script>
<!-- script js / -->