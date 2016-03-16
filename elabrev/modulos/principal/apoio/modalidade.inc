<script type="text/javascript">
	var codigos = new Array(); 
</script>
<?php
include APPRAIZ . "includes/cabecalho.inc";

/*******************************
* 	VARIAVEIS
*	Recebe dados do formulario 
*******************************/
$pstNum			= count($_POST);
$pstIncluir		= $_POST["inserir"];
$pstExcluir		= $_POST["excluir"];
$pstEditar		= $_POST["editar"];
$pstCodigo		= $_POST["codigo"];
$pstDescricao	= $_POST["descricao"];

/*******************************
* 	AÇÃO
*	insere, edita e exclui da 
*   tabela de modalidade 
*******************************/
if($pstNum && $pstIncluir){
	$sql="INSERT INTO financeiro.tipomodalidade (tpmcod,tpmdsc) VALUES ('".$pstCodigo."','".$pstDescricao."')";
	$db->executar( $sql );
	$alerta = "Modalidade incluida com sucesso.";
}

if($pstNum && $pstEditar){
	$sql="UPDATE financeiro.tipomodalidade SET tpmcod = '".$pstCodigo."',tpmdsc = '".$pstDescricao."' WHERE tpmid = ".$pstEditar.";";
	$db->executar( $sql );
	$alerta = "Modalidade alterada com sucesso.";
}

if($pstNum && $pstExcluir){
	$sql="DELETE FROM financeiro.tipomodalidade WHERE tpmid=".$pstExcluir;
	$db->executar( $sql );
	$alerta = "Modalidade excluida com sucesso.";
}

if($pstNum){
	echo "<script> 
			alert( '".$alerta."');
    	  </script>";
	$db->commit();
}

/*******************************
* 	MONTA LISTA
*	Recupera a lista de modalidade 
*   e monta a lista.
*******************************/
$sql= "	SELECT
			'<a href=\"#\"><img onclick=\"editar('||tpmid||',\''||tpmcod||'\',\''||tpmdsc||'\')\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> '
			'<a style=\"margin: 0 0 0 5px;\" href=\"\"> <img onclick=\"excluir('||tpmid||')\" src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>'as acoes,
			tpmcod, 
			tpmdsc 
		FROM financeiro.tipomodalidade";
$cabecalho = array ("Ações ","Código", "Descrição");
$sqlvalidacao= "SELECT tpmcod FROM financeiro.tipomodalidade";
$codexiste = $db->carregar($sqlvalidacao);
if($codexiste != NULL){
foreach($codexiste as $chave=>$codigo){
?>
<script type="text/javascript">
	codigos[<?=$chave ?>] = <?=$codigo['tpmcod']?>; 
</script>
<?	
	}
}	
?>
<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
    <form name="formulario" method="post">
	    <input type='hidden' name='inserir' value=0>
	    <input type='hidden' name='excluir' value=0>
	    <input type='hidden' name='editar' value=0>
		<tr>
		<th colspan="2"><h1>Tabela de convenção nível / modalidade</h1></th>
		</tr>
		<tr>
		<td class="SubTituloDireita">Código:</td>
		<td><input style="width:320px;" type="text" name="codigo" id="codigo" /></td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Descrição:</td>
		<td> <input style="width:320px;" type="text" name="descricao" id="descricao" /></td>
		</tr>
		<tr>
		<th colspan="2"> 
			<input type="button" name="incluir" value="incluir" onclick="inserirdados('novo');"/>
			<input style="display:none" type="button" name="alterar" value="alterar"  onclick="inserirdados('alterar');" />
			<input type="button" name="voltar" value="voltar"  onclick="voltarpag();"  />
		</th>
		</tr>
	</form>
</table>
<?
/*******************************
* 	MONTA LISTA
*******************************/
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'left', '' );
?>
<script type="text/javascript">
	/*Submete os dados.*/
	var codigoinicial ="";
	function inserirdados(valor) {
		descricao 	= document.getElementById('descricao').value;
			cod 	= document.getElementById('codigo').value;
		if(descricao == "" ){
			alert("O campo descrição é obrigatorio");
			return false;
		}
		if(cod == "" ){
			alert("O campo código é obrigatorio");
			return false;
		}
		if(cod > 9 ){
			alert("O campo código aceita apenas carachters 0 a 9 ");
			return false;
		}
		for(i=0; i<codigos.length; i++){
			if(codigos[i]== cod && codigoinicial != cod ){
				alert("Este código já está cadastrado para outra convenção.");
				return false;
			}
		}
		if(valor == 'novo'){
			document.formulario.inserir.value = 1;
		}
		document.formulario.submit();
	}
	
	/*Exclui a convenção pelo código*/
	function excluir(cod) { 
		if( window.confirm( "Confirma a exclusão do ítem "+ cod + " de convenção?") ){
			document.formulario.excluir.value = cod;
			document.formulario.submit();
	    }else document.formulario.excluir.value = 0;
	}
	
	/*Edita a convenção pelo codigo*/
	function editar(id, cod, desc){
		document.formulario.editar.value = id;
		document.formulario.descricao.value = desc;
		document.formulario.incluir.style.display="none";
		document.formulario.codigo.value = cod;
		document.formulario.alterar.style.display="inline";
		codigoinicial=document.getElementById('codigo').value;
	}
	
	/*BTN Voltar */
	function voltarpag(){
		if(document.formulario.incluir.style.display == "none"){
			document.formulario.incluir.style.display="inline";
			document.formulario.alterar.style.display="none";
			document.formulario.descricao.value = "";
			document.formulario.codigo.value = "";
			document.formulario.editar.value = 0;
		}else{
			history.back();
		}
	}
</script>