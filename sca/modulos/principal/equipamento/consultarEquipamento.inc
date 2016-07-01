<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$titulo = "Consulta de Equipamentos";
monta_titulo( $titulo, '&nbsp;' );

include APPRAIZ."sca/classes/Equipamento.class.inc";
$oEquipamento = new Equipamento();

if(!empty($_REQUEST['excluir'])){
	$oEquipamento->excluirEquipamento($_REQUEST['excluir']);
}
$responsavel_nome  = $_REQUEST['responsavel_nome'];//
$eqmnumserie       = $_REQUEST['eqmnumserie'];
$eqstatus          = $_REQUEST['eqstatus'];
$eqmnumetiqueta    = $_REQUEST['eqmnumetiqueta'];

?>

<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
	//função que dispara a pesquisa no banco
	function pesquisar(acao){
		var url = 'sca.php?modulo=principal/equipamento/consultarEquipamento&acao=A&pesquisar='+acao;
		var formulario=document.getElementById("formulario");
		formulario.action = url;
		formulario.submit();
	}

	//função que chama a edição do registro
	function detalharEquipamento(equipamentoId){
		window.location = "?modulo=principal/equipamento/cadastrarEquipamento&acao=A&eqmid="+equipamentoId;

	}

	//função que chama a exclusão do registro
	function removerEquipamento(equipamentoId){
		window.location = "?modulo=principal/equipamento/consultarEquipamento&acao=A&excluir="+equipamentoId;

	}


</script>

<form name="formulario" id="formulario" method="post" action="sca.php?modulo=principal/equipamento/consultarEquipamento&acao=A&pesquisar=filtrar">
	<table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
		<tr>
			<td class="SubTituloDireita" width="200">Número da Etiqueta:</td>
			<td><?=campo_texto('eqmnumetiqueta','','','',21,60,'','','left','',0,'id="eqmnumetiqueta"');?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="200">Número de Série:</td>
			<td><?=campo_texto('eqmnumserie','','','',21,60,'','','left','',0,'id="eqmnumserie"');?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="200">Status:</td>
            <td>
                <select id="eqstatus" name="eqstatus">
                    <option <?php if($eqstatus==0)       echo ' selected'; ?> value="0" >Selecione</option>                              
                    <option <?php if($eqstatus=='volvo') echo ' selected'; ?> value="volvo">Dentro do Mec</option>
                    <option <?php if($eqstatus=='saab')  echo ' selected'; ?> value="saab">Fora do Mec</option>
                </select>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="200">Responsável:</td>
			<td><?=campo_texto('responsavel_nome','','','',21,60,'','','left','',0,'id="responsavel_nome"');?></td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC" colspan="2" align="center">
				<input type="submit" name="visualizar" value="Visualizar" style="cursor:pointer;">
				<input type="button" name="vertodos" value="Ver Todos" style="cursor:pointer;" onclick="pesquisar('todos');">
			</td>
		</tr>
	</table>
</form>
<script>
    Event.observe(window, 'load', function() {
        $("eqmnumetiqueta").focus();
      });
</script>

<?php

//verifica se é para disparar a consulta no banco
if(!empty($_REQUEST['pesquisar'])){
     //ver($_REQUEST);
	if($_REQUEST['pesquisar'] =='filtrar'){
		$oEquipamento->listarEquipamentoPorFiltro( $_REQUEST );
	}elseif ($_REQUEST['pesquisar'] =='todos'){
		$oEquipamento->listarEquipamentoPorFiltro();
       //        ver($_REQUEST);
	}
}
?>
<script type="text/javascript">
	(function(){$$('.listagem td.title').invoke('removeAttribute','onclick');})();
</script>