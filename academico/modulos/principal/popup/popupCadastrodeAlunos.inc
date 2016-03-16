<?php 

function listaAlunos( $ofcid ){
	
	global $db;
	
	$sql = "SELECT 
				damid, 
				damano, 
				damsemestre, 
				dammatriculados, 
				damconcluintes
			FROM 
				academico.dadosalunos
			WHERE
				ofcid = {$ofcid}
			ORDER BY
				damano, damsemestre";

	$alunos = $db->carregar($sql);
	
?>
			<table width="60%" class="Tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
				<tr style="background-color: rgb(220, 220, 220);">
					<td width="10%" align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
						<b> Ação </b>
					</td>
					<td width="14%" align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
						<b> Ano </b>
					</td>
					<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
						<b> Semestre </b>
					</td>
					<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"class="title">
						<b> Alunos Matriculados </b>
					</td>
					<td width="28%" align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
						<b> Alunos Concluintes </b>
					</td>
				</tr>
			</table>
			<br>
			<br>
			<div align="left" style="width: 100%; height: 200px; overflow: auto">
				<table width="100%" class="Tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
<?php 
	$i=0;
	if( is_array($alunos) ){
		foreach( $alunos as $aluno ){
			
			if( $i % 2 == 1 ){
				$cor = 'rgb(235, 235, 235);';
				$i++;
			}else{
				$cor = 'rgb(255, 255, 255);';
				$i++;
			}
			
			$json = simec_json_encode($aluno);
			
?>
					<tr style="background-color: <?=$cor; ?>">
						<td align="center" width="10%">
							<img border="0" onclick='carregaAluno(<?=$json;?>);'           style="cursor: pointer;" title="Editar" src="/imagens/alterar.gif">
							<img border="0" onclick='excluirAluno(<?=$aluno['damid'];?>);' style="cursor: pointer;" title="Editar" src="/imagens/excluir.gif">
						</td>
						<td align="center" width="15%">
							<?=$aluno['damano'] ?>
						</td>
						<td align="center" width="17%">
							<?=$aluno['damsemestre'] ?>°
						</td>
						<td align="center" width="32%" style="color: blue">
							<?=number_format($aluno['dammatriculados'],0,',','.') ?>
						</td>
						<td align="center" style="color: blue">
							<?=number_format($aluno['damconcluintes'],0,',','.') ?>
						</td>
					</tr>
<?php 		
		}
	}else{
?>
					<tr>
						<td colspan="12" style="color: red">
							Não existem registros cadastrados.
						</td>
					</tr>
<?php 		
	}
?>
				</table>
			</div>
<?php 
}

function gravaAluno( $dados ){
	
	global $db;
    
	//Tratamento de valores do Padrão brasileiro para padrão Americano(do Banco)
	$dados['dammatriculados']      = str_replace(".","",$dados['dammatriculados']);
	$dados['damconcluintes']   	   = str_replace(".","",$dados['damconcluintes']);
	
    //Se possuir ofcid, atualiza, se não, insere
	if( $dados['damid'] ){
		
		$sql = "UPDATE academico.dadosalunos
				SET 
					damano 		 	= {$dados['damano']}, 
					damsemestre 	= {$dados['damsemestre']}, 
					dammatriculados = {$dados['dammatriculados']}, 
					damconcluintes  = {$dados['damconcluintes']}
				WHERE 
					damid = {$dados['damid']}";
		
	}else{
		
		$sql = "INSERT INTO 
					academico.dadosalunos(
			            ofcid, 
			            damano, 
			            damsemestre, 
			            dammatriculados, 
			            damconcluintes)
				VALUES (
						{$dados['ofcid']}, 
						{$dados['damano']}, 
						{$dados['damsemestre']}, 
						{$dados['dammatriculados']}, 
						{$dados['damconcluintes']})";
		
	}
	
	//Se não possuir erro comita.
	if($db->executar($sql)){
		$db->commit();
		echo "<script>alert('Dados salvos com sucesso!');</script>";
	}

}

// Gravar Aluno
if( $_REQUEST['req'] == 'gravar' ){
	gravaAluno( $_REQUEST );
	unset($_REQUEST['req']);
}

// Excluir Aluno
if( $_REQUEST['req'] == 'excluir' ){
	
	$sql = "DELETE FROM
				academico.dadosalunos
			WHERE
				damid = {$_REQUEST['damid']}";
	
	$db->executar($sql);
	$db->commit();
	
	echo listaAlunos($_REQUEST['ofcid']);
	die();	
}

monta_titulo( "Cadastro de Alunos", "");

?>
<script type="text/javascript" src="/includes/prototype.js"></script>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel="stylesheet" href="/includes/entidadesn.css" type="text/css" media="screen" />

<form name="frmCursos" id="frmCursos" method="post" action="" onsubmit="">

	<input type="hidden" id="ofcid" 	  name="ofcid" 		 value="<?=$_REQUEST['ofcid'] ?>">
	<input type="hidden" id="damid" 	  name="damid" 		 value="">
	<input type="hidden" id="req"   	  name="req"   		 value="">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tblentidade">
		<tr>
			<td></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Semestre :
			</td>
			<td>
				<input type="radio" name="damsemestre" id="damsemestre1" value="1" checked="checked" /> 1° Semestre
				<input type="radio" name="damsemestre" id="damsemestre2" value="2" /> 2° Semestre
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Ano :
			</td>
			<td>
				<?= campo_texto( 'damano', 'S', 'S', '', 4, 6, '####', '','','','','id="damano"','','','this.value=mascaraglobal(\'####\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Quantidade de Matriculados :
			</td>
			<td>
				<?= campo_texto( 'dammatriculados', 'S', 'S', '', 25, 500, '[###.]###', '','','','','id="dammatriculados"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Quantidade de Concluintes :
			</td>
			<td>
				<?= campo_texto( 'damconcluintes', 'S', 'S', '', 25, 500, '[###.]###', '','','','','id="damconcluintes"','','','this.value=mascaraglobal(\'[###.]###\',this.value);'  ); ?>
			</td>
		</tr>
		<tr>
			<td> 
			</td>
			<td>
				<input type="button" value="Gravar" onclick="validaForm();"></input>
			</td>
		</tr>
		<tr>
			<td></td>
			<td id="tdAlunos" height="200px">
				
<?php 

	echo listaAlunos( $_REQUEST['ofcid'] );
	
?>
			</td>
		</tr>
	</table>
</form>
<script>

var ofcid = <?=$_REQUEST['ofcid'] ?>

function carregaAluno( dados ){
	
	var damid   = $('damid');
	damid.value = dados.damid;

	var damsemestre1   = $('damsemestre1');
	var damsemestre2   = $('damsemestre2');
	if( dados.damsemestre == '1' ){
		damsemestre1.checked = true;
	}else{
		damsemestre2.checked = true;
	}
	 
	var damano   = $('damano');
	damano.value = dados.damano; 
	
	var dammatriculados   = $('dammatriculados');
	dammatriculados.value = dados.dammatriculados;

	var damconcluintes   = $('damconcluintes');
	damconcluintes.value = dados.damconcluintes;

}

function excluirAluno( damid ){

	var td = $('tdAlunos')
	
	if( confirm('Realmente deseja excluir este curso?') ){
		return new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: '&req=excluir&damid=' + damid + '&ofcid=' + ofcid,
			onComplete: function(res){
				td.innerHTML = res.responseText;
				alert('Aluno excluído!');
			}
		});
	}

}

function validaForm(){

	var form 		    = $('frmCursos');
	var req 		    = $('req');
	var damsemestre1    = $('damsemestre1');
	var damsemestre2    = $('damsemestre2');
	var damano 		    = $('damano');
	var dammatriculados = $('dammatriculados');
	var damconcluintes  = $('damconcluintes');
	
	if( !damsemestre1.checked && !damsemestre2.checked ){
		alert('Item obrigatório.');
		damsemestre1.focus();
		return false;
	}
	
	if( damano.value == '' ){
		alert('Item obrigatório.');
		damano.focus();
		return false;
	}
	
	if( dammatriculados.value == '' ){
		alert('Item obrigatório.');
		dammatriculados.focus();
		return false;
	}
	
	if( damconcluintes.value == '' ){
		alert('Item obrigatório.');
		damconcluintes.focus();
		return false;
	}
	
	req.value = 'gravar';
	form.submit();
}

</script>


