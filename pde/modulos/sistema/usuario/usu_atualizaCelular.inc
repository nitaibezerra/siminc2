<? 

function atualizaCelulares( $request ){
	
	global $db;
	
	extract($request);
	
	if( is_array($ddd) ){
		$sql = '';
		foreach( $ddd as $entid => $dd ){
			if( $dd != '' && $cel[$entid] != '' ){
				$sql .= "UPDATE entidade.entidade SET
							entnumdddcelular = '$dd',
							entnumcelular = '".$cel[$entid]."'
						 WHERE
							entid = $entid;";
			}
		}	
		if( $sql != '' ){
			$db->executar($sql);
			$db->commit();	
			echo "<script>alert('Numeros de clelular atualizados.');</script>";
		}
	}
	echo "<script>window.location = window.location</script>";
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

include APPRAIZ."includes/cabecalho.inc"; 
?>
<br>
<? monta_titulo($titulo_modulo,''); ?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
$(document).ready(function(){
	$('#salvar').click(function(){
		$('#formulario').submit();
	});
});
</script>
<form name="formulario" id="formulario" method="post">
	<input type="hidden" name="req" id="req" value="atualizaCelulares"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#C0C0C0">
	  		<td><b>CPF</b></td>
	  		<td><b>Nome</b></td>
	  		<td><b>DDD</b></td>
	  		<td><b>Celular</b></td>
		</tr>
		<?php 
		$sql = "SELECT DISTINCT
					replace(to_char(entnumcpfcnpj::numeric,'000,999,999-99'),',','.') as cpf,
					entnome as nome,
					entnumdddcelular as ddd,
					entnumcelular as cel,
					ent.entid as entid
				FROM 
					entidade.entidade ent
				INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
				INNER JOIN entidade.funcao fun ON fun.funid = fen.funid
				WHERE
					fun.funid IN (98,100) AND entstatus = 'A'
				ORDER BY
					2";
		$usuarios = $db->carregar($sql);
		foreach( $usuarios as $k => $usuario ){
			extract($usuario);
			$cor = ($k%2)==0 ? 'bgcolor="white"' : '';
		?>
		<tr <?=$cor ?>>
	  		<td><?=$cpf ?></td>
	  		<td><?=$nome ?></td>
	  		<td>
	  			<?=campo_texto('ddd['.$entid.']', 'N', 'S', 'DDD', 1, 2, '##', '', '', '', '', '', '', $ddd) ?>
	  		</td>
	  		<td>
	  			<?=campo_texto('cel['.$entid.']', 'N', 'S', 'DDD', 8, 9, '####-####', '', '', '', '', '', '', $cel) ?>
	  		</td>
		</tr>
		<?php 	
		}
		?>
		<tr bgcolor="#C0C0C0">
	  		<td colspan="4">
	  			<b>Total de registros: <?=count($usuarios) ?></b>
	  			<input type="button" style="float:right" value="Atualizar Numeros de Celular" id="salvar"/>
	  		</td>
		</tr>
	</table>
</form>