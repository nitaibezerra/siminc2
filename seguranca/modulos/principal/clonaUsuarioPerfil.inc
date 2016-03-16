<?
ini_set("memory_limit", "2048M");
set_time_limit(0);

function clonarPerfil($arrDados)
{
	global $db;
	extract($_POST);
	$sql = "select count(distinct usuario.usucpf) as total,perfil.pfldsc,sisabrev 
			from seguranca.perfil perfil 
			inner join seguranca.perfilusuario perfilusuario on perfil.pflcod = perfilusuario.pflcod
			right join seguranca.usuario usuario on usuario.usucpf = perfilusuario.usucpf
			inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf
			inner join seguranca.sistema sistema on sistema.sisid = usuariosistema.sisid
			where usuariosistema.sisid = $sisidmaster and perfil.pflcod = $pflcodmaster
			group by perfil.pfldsc,sisabrev";
	$arrDados = $db->pegaLinha($sql);
	$sql = "select pfldsc from seguranca.perfil where pflcod = $pflcodslave";
	$pfldsc = $db->pegaUm($sql);
	echo "<p>Serão inseridos <u>{$arrDados['total']}</u> usuários com o perfil de <u>$pfldsc</u> no módulo <u>{$arrDados['sisabrev']}</u>. Confirma?</p>";
	echo "<input type='button' name='btn_sim' value='Sim' onclick='confirmaClonagem()' /> ";
	echo "<input type='button' name='btn_nao' value='Não' onclick='cancelaClonagem()' />";
	echo "<input type='button' name='btn_carregando' value='Aguarde...Carregando' disabled='disabled' style='display:none' />";
}

function confirmaClonagem($arrDados)
{
	global $db;
	extract($_POST);
	$pflcodmaster = !$pflcodmaster ? "null" : $pflcodmaster;
	$pflcodslave = !$pflcodslave ? "null" : $pflcodslave;
	$sql = "insert into seguranca.perfilusuario (usucpf,pflcod) 
			select usucpf,$pflcodslave from seguranca.perfilusuario where pflcod = $pflcodmaster  and usucpf not in (select usucpf from seguranca.perfilusuario where pflcod = $pflcodslave);";
	$sql.= "insert into seguranca.usuario_sistema (usucpf,sisid,susstatus,pflcod,susdataultacesso,suscod)
			select usucpf,$sisidslave,susstatus,$pflcodslave,susdataultacesso,suscod from seguranca.usuario_sistema where sisid = $sisidmaster and pflcod = $pflcodmaster and usucpf not in (select usucpf from seguranca.usuario_sistema where sisid = $sisidslave);";
	$sql.= "insert into seguranca.usuario_sistema (usucpf,sisid,susstatus,pflcod,susdataultacesso,suscod)
			select usucpf,$sisidslave,susstatus,null,susdataultacesso,suscod from seguranca.usuario_sistema where sisid = $sisidmaster and pflcod is null and usucpf not in (select usucpf from seguranca.usuario_sistema where sisid = $sisidslave);";
	
	$sqlResp = "SELECT DISTINCT
					pg_attribute.attname AS codigo,
					pg_attribute.attname AS descricao
				FROM 
					pg_class
				JOIN 
					pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE 'pg_%'
				JOIN 
					pg_attribute ON pg_attribute.attrelid = pg_class.oid AND pg_attribute.attisdropped = 'f'
				JOIN
					pg_type ON pg_type.oid = pg_attribute.atttypid
				JOIN 
					pg_index ON pg_index.indrelid=pg_class.oid
				LEFT JOIN
					pg_constraint ON (pg_attribute.attrelid = pg_constraint.conrelid AND pg_constraint.conkey[1] = pg_attribute.attnum AND pg_constraint.contype != 'u')
				WHERE 
					pg_class.relname = 'usuarioresponsabilidade'
				AND
					pg_namespace.nspname IN( SELECT sisdiretorio FROM seguranca.sistema WHERE sisstatus='A' AND sisid='".$sisidmaster."' )
				AND 
					pg_attribute.attnum > 0
				AND 
					pg_attribute.attrelid = pg_class.oid
				AND 
					pg_attribute.atttypid = pg_type.oid 
				AND 
					pg_attribute.attname NOT IN('rpuid','usucpf','rpudata_inc','rpustatus','pflcod')
					
			INTERSECT ALL (
			
				SELECT DISTINCT
					pg_attribute.attname AS codigo,
					pg_attribute.attname AS descricao
				FROM 
					pg_class
				JOIN 
					pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE 'pg_%'
				JOIN 
					pg_attribute ON pg_attribute.attrelid = pg_class.oid AND pg_attribute.attisdropped = 'f'
				JOIN
					pg_type ON pg_type.oid = pg_attribute.atttypid
				JOIN 
					pg_index ON pg_index.indrelid=pg_class.oid
				LEFT JOIN
					pg_constraint ON (pg_attribute.attrelid = pg_constraint.conrelid AND pg_constraint.conkey[1] = pg_attribute.attnum AND pg_constraint.contype != 'u')
				WHERE 
					pg_class.relname = 'usuarioresponsabilidade'
				AND
					pg_namespace.nspname IN( SELECT sisdiretorio FROM seguranca.sistema WHERE sisstatus='A' AND sisid='".$sisidslave."' )
				AND 
					pg_attribute.attnum > 0
				AND 
					pg_attribute.attrelid = pg_class.oid
				AND 
					pg_attribute.atttypid = pg_type.oid 
				AND 
					pg_attribute.attname NOT IN('rpuid','usucpf','rpudata_inc','rpustatus','pflcod')
			
			)";
	$arrUresp = $db->carregarColuna($sqlResp);
	if($arrUresp){
		$sql_s1 = "SELECT sisdiretorio FROM seguranca.sistema WHERE sisstatus='A' AND sisid='".$sisidmaster."'";
		$sistema_master = $db->pegaUm($sql_s1);
		$sqlContem = "select * from $sistema_master.usuarioresponsabilidade where pflcod = $pflcodmaster limit 1";
		$existe_ure = $db->pegaUm($sqlContem);
		if($existe_ure){
			$sql_s2 = "SELECT sisdiretorio FROM seguranca.sistema WHERE sisstatus='A' AND sisid='".$sisidslave."'";
			$sistema_slave = $db->pegaUm($sql_s2);
			$sql.="insert into $sistema_slave.usuarioresponsabilidade (pflcod,usucpf,rpustatus,rpudata_inc,".implode(",",$arrUresp).") 
					select $pflcodslave,usucpf,rpustatus,rpudata_inc,".implode(",",$arrUresp)." from $sistema_master.usuarioresponsabilidade where pflcod = $pflcodmaster";
		}
	}
	echo "Aguarde...Executando clonagem.";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Operação realizada com sucesso.');window.location.href='seguranca.php?modulo=principal/clonaUsuarioPerfil&acao=A';</script>";
}

function carregarPerfil($dados) {
	global $db;
	
	$sql = "SELECT pflcod as codigo, pfldsc as descricao FROM seguranca.perfil WHERE pflstatus='A' AND sisid='".$dados['sisid']."' ORDER BY pfldsc";
	$db->monta_combo($dados['name'], $sql, 'S', 'Selecione', '', '', '', '', 'S', $dados['name'], false, $dados['pflcodsel']);
}
if($_REQUEST['requisicaoAjax']) {
	$_REQUEST['requisicaoAjax']($_REQUEST);
	exit;
}
if($_POST){
	extract($_POST);
}
include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo( "Clona usuários por perfil", "Os usuários serão vinculados ao perfil de destino com as devidas responsabilidades.");

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="./js/seguranca.js"></script>
<script>
jQuery.noConflict();

function carregarPerfilMaster(sisid) {
	jQuery.ajax({
   		type: "POST",
   		url: "seguranca.php?modulo=principal/clonaUsuarioPerfil&acao=A",
   		data: "requisicaoAjax=carregarPerfil&name=pflcodmaster<?=(($espelhoperfil['pflcodmaster'])?"&pflcodsel=".$espelhoperfil['pflcodmaster']:"") ?>&sisid="+sisid,
   		async: false,
   		success: function(html){jQuery('#td_perfilmaster').html(html);}
 		});
}

function carregarPerfilSlave(sisid) {
	jQuery.ajax({
   		type: "POST",
   		url: "seguranca.php?modulo=principal/clonaUsuarioPerfil&acao=A",
   		data: "requisicaoAjax=carregarPerfil&name=pflcodslave<?=(($espelhoperfil['pflcodslave'])?"&pflcodsel=".$espelhoperfil['pflcodslave']:"") ?>&sisid="+sisid,
   		async: false,
   		success: function(html){jQuery('#td_perfilslave').html(html);}
 		});
}

function clobarPerfil() {
	if(jQuery('#sisidmaster').val()=='') {
		alert('Selecione o sistema de origem');
		return false;
	}
	if(jQuery('#pflcodmaster').val()=='') {
		alert('Selecione o perfil de origem');
		return false;
	}
	if(jQuery('#sisidslave').val()=='') {
		alert('Selecione o sistema de destino');
		return false;
	}
	if(jQuery('#pflcodslave').val()=='') {
		alert('Selecione o perfil de destino');
		return false;
	}
	
	document.getElementById('formulario').submit();
}

function confirmaClonagem()
{
	jQuery("[name='requisicao']").val("confirmaClonagem");
	jQuery("[name='btn_sim']").hide();
	jQuery("[name='btn_nao']").hide();
	jQuery("[name='btn_carregando']").show();
	document.getElementById('formulario').submit();
}

function cancelaClonagem()
{
	window.location.href='?modulo=principal/clonaUsuarioPerfil&acao=A';
}

</script>
<form method="post" name="formulario" id="formulario">
<input type="hidden" name="requisicao" value="clonarPerfil">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'>Sistema de Origem:</td>
	<td>
	<?
	$sql = "SELECT sisid as codigo, sisabrev as descricao FROM seguranca.sistema WHERE sisstatus='A' order by sisabrev";
	$db->monta_combo('sisidmaster', $sql, 'S', 'Selecione', 'carregarPerfilMaster', '', '', '', 'S', 'sisidmaster', false, $espelhoperfil['sisidmaster']);
	?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Perfil de Origem:</td>
	<td id="td_perfilmaster">
	<?php if($sisidmaster): ?>
			<?php carregarPerfil(array("name"=>"pflcodmaster","sisid"=>$sisidmaster)) ?>
	<?php else: ?>
		<b>Selecione sistema de destino</b>
	<?php endif; ?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Sistema de Destino:</td>
	<td>
	<?
	$sql = "SELECT sisid as codigo, sisabrev as descricao FROM seguranca.sistema WHERE sisstatus='A' order by sisabrev";
	$db->monta_combo('sisidslave', $sql, 'S', 'Selecione', 'carregarPerfilSlave', '', '', '', 'S', 'sisidslave', false, $espelhoperfil['sisidslave']);
	?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Perfil de Destino:</td>
	<td id="td_perfilslave">
		<?php if($sisidslave): ?>
			<?php carregarPerfil(array("name"=>"pflcodslave","sisid"=>$sisidslave)) ?>
		<?php else: ?>
			<b>Selecione sistema de destino</b>
		<?php endif; ?>
	</td>
</tr>
<?php if(!$_REQUEST['requisicao']): ?>
	<tr>
		<td class='SubTituloCentro' colspan="2"><input type="button" name="botao" value="Clonar" onclick="clobarPerfil();"></td>
	</tr>
<?php endif; ?>
</table>
</form>
<?php if($_REQUEST['requisicao']): ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class='SubTituloCentro'>
	<?php
	if($_REQUEST['requisicao']) {
		$_REQUEST['requisicao']($_REQUEST);
	}
	?>
	</td>
		</tr>
	</table>
<?php endif; ?>