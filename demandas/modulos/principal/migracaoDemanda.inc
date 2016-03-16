<?php



/*
 * Executa a função que salva 
 * Retorna mensagem de sucesso e redireciona a página
 */
if ($_POST['usucpforigem'] && $_POST['usucpfdestino']){

	$sql = "select d.dmdid from demandas.demanda d 
			LEFT JOIN workflow.documento doc ON doc.docid = d.docid
			where d.usucpfdemandante = '{$_POST['usucpforigem']}'
			AND d.dmdstatus = 'A'	
		    AND ( doc.esdid is null OR doc.esdid in (91,107,92,108) ) ";
	$dados = $db->carregar($sql);	
	
	if($dados){
		foreach($dados as $d){
		    $sql = "UPDATE demandas.demanda SET 
		    			usucpfdemandante = '{$_POST['usucpfdestino']}',
		    			usucpfmigracao = '{$_SESSION['usucpf']}', 
		    			dmddatamigracao = now()
		    		WHERE dmdid = {$d['dmdid']}";
		    $db->executar($sql);
		}
	}

	$db->commit();
 	
	die("<script>
			alert('Operação realizada com sucesso!');
			location.href='demandas.php?modulo=principal/migracaoDemanda&acao=A';
		 </script>");
    	
}

	

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Migração de Demandas', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );


?>
<html>

<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>

<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">


<form name="formulario" action="" method="post">

<table width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">DEMANDAS EM ABERTO DE:</td>
		<td>
		<?php 
				$sql = "SELECT
						 u.usucpf AS codigo,
						 u.usunome AS descricao
						FROM
						 seguranca.usuario u
						 inner join seguranca.usuario_sistema us on
						 u.usucpf = us.usucpf
						 where
						 us.sisid = ".$_SESSION['sisid']." AND
						 us.susstatus = 'A' AND
						 us.suscod = 'A'
						ORDER BY
						 u.usunome;";
				$db->monta_combo("usucpforigem",$sql, 'S' ,"-- Selecione um usuário --",'','');		
		?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">MIGRAR PARA:</td>
		<td>
					<?php 
				$sql = "SELECT
						 u.usucpf AS codigo,
						 u.usunome AS descricao
						FROM
						 seguranca.usuario u
						 inner join seguranca.usuario_sistema us on
						 u.usucpf = us.usucpf
						 where
						 us.sisid = ".$_SESSION['sisid']." AND
						 us.susstatus = 'A' AND
						 us.suscod = 'A'
						ORDER BY
						 u.usunome;";
				$db->monta_combo("usucpfdestino",$sql, 'S' ,"-- Selecione um usuário --",'','');		
		?>
		
		</td>
	</tr>	
	<tr bgcolor="#C0C0C0">
		<td colspan="2" align="center">
			<input type="button" class="botao" name="btalterar" id="btalterar" value="Executar" onclick="validaForm();">
		</td>
	</tr>	
</table>
</form>
</body>
</html>

<script type="text/javascript">
d = document.formulario;


/*
* Function destinada a validação do formulário
*/
function validaForm(){
	
	if (d.usucpforigem.value == ''){
		alert('O campo DEMANDA DE é obrigatório!');
		d.usucpforigem.focus();
		return false;
	}
	if (d.usucpforigem.value == ''){
		alert('O campo MIGRAR PARA é obrigatório!');
		d.usucpforigem.focus();
		return false;
	}

	if(confirm('Deseja realmente executar esta operação?')){
		d.btalterar.disabled = true;
		d.submit();
	}
}
</script>
