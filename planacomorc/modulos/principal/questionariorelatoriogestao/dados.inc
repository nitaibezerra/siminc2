<?
if($_SESSION ['planacomorc'] ['tqtid'] != null){
$monsubacaquestionario = $db->pegaLinha ( "
	SELECT *
    FROM planacomorc.tcuquestionario
    WHERE tqtid='" . $_SESSION ['planacomorc'] ['tqtid'] . "'" );
}
?>
<script>
function gravarQuestionario(){
	if(verificaFormulario())	    
    	$('#formquestionario').submit();
}
function verificaFormulario(){
	if($('#tqtdsc').val()=='') {
        alert('Nome do questionário em branco');
        return false;
    }

    if($('#qtdexercicio').val()=='') {
        alert('Exercício em branco');
        return false;
    }

    return true;
}

function atualizarQuestionario() {
	if(verificaFormulario())
    	$('#formquestionario').submit();
    
}
</script>
<section class="col-md-12">
	<section class="well">
		<form class="form-horizontal" method="post" name="formquestionario" id="formquestionario">			
			<?php if ($_SESSION['planacomorc']['tqtid'] != null){
				?><input type="hidden" name="requisicao" value="atualizarQuestionario"/><?php
			}else{
				?><input type="hidden" name="requisicao" value="inserirQuestionario"/><?php 
			}			
			?>
			
			<section class="form-group">
				<label class="control-label col-md-2" for='tqtdsc'>Nome:</label>
				<section class="col-md-10">
					<?php inputTexto('tqtdsc', $monsubacaquestionario['tqtdsc'], 'tqtdsc', 150,false);?>
				</section>
			</section>	
			
			<section class="form-group">
				<label class="control-label col-md-2" for="qtdexercicio">Exercício:</label>
				<section class="col-md-10">
					<?php
					for($i = 2014; $i <= 2020; $i++){
						$dados[] = array('codigo' => $i,'descricao' => $i);
					}					
					inputCombo('qtdexercicio', $dados, $monsubacaquestionario['qtdexercicio'], "qtdexercicio",array('titulo'=>'Selecione'));
					?>			
				</section>
			</section>	
			
			<section class="form-group">
				<section class="col-md-offset-2">
					<?php 
					if($_SESSION ['planacomorc'] ['tqtid'] == null){
					?>
					<input type="button" class="btn btn-success" name="questionario" value="Cadastrar" onclick="gravarQuestionario();">
					<?php 
					}else{
					?>
					<input type="button" class="btn btn-primary" name="questionario" value="Salvar"	onclick="atualizarQuestionario();">
					<?php 
					}										
					?>
						
				</section>
			</section>            
		</form>
	</section>
</section>