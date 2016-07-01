<?
function bloquearPagamento($dados){
	global $db ;

	$estcpf = str_replace(Array('.','-'),'',$dados['cpf']);

	$sql="UPDATE projovemcampo.estudante
			SET
				bloqueadopagamento ='t'
			WHERE
				estcpf = '{$estcpf}'";

	$db->executar($sql);
	$db->commit();

	echo "<script>
				window.location.href = window.location.href;
			 </script>";
	die;
}

function desbloquearPagamento($dados){
	global $db;

	$sql="UPDATE projovemcampo.estudante
			SET
				bloqueadopagamento ='f'
			WHERE
				estid = '{$dados['estid']}'";

	$db->executar($sql);
	$db->commit();

	echo "<script>
				window.location.href = window.location.href;
			 </script>";
	die;
}

if( $_REQUEST['requisicao'] ){
	$_REQUEST['requisicao']($_REQUEST);
	die();
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ . 'includes/cabecalho.inc';
$perfis = pegaPerfilGeral();

if(in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)|| in_array(PFL_ADMINISTRADOR, $perfis)) {
	$menu = array(0 => array("id" => 1, "descricao" => "Painel", 			"link" => "/projovemcampo/projovemcampo.php?modulo=inicio&acao=C"),
		1 => array("id" => 2, "descricao" => "Painel Pagamento", 	"link" => "/projovemcampo/projovemcampo.php?modulo=principal/pagamento/painelPagamento&acao=A"),
		2 => array("id" => 3, "descricao" => "Bloquear Pagamento", 	"link" => "/projovemcampo/projovemcampo.php?modulo=principal/pagamento/bloquearPagamento&acao=A")
	);
}
else{
	$menu = array(0 => array("id" => 1, "descricao" => "Documentos", 	"link" => "/projovemcampo/projovemcampo.php?modulo=principal/anexoGeral&acao=A")
	);
}


echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
$db->cria_aba( $abacod_tela, $url, '');
echo '<br />';

?>

<script language="javascript" type="text/javascript">

jQuery(document).ready(function()
{
	jQuery('.bloquear').live('click',function(){

		var estcpf = replaceAll(replaceAll(jQuery("[name='cpf']").val(), ".", ""), "-", "");

		confirm('Tem certeza que deseja bloquear o pagamento este CPF?');

		if(estcpf == ''){
			alert('Insira um CPF!');
			return false;
		}

		jQuery('#requisicao').val('bloquearPagamento');
		jQuery('#form').submit();
	});
	jQuery('.desbloquear').live('click',function(){

		var estid = jQuery(this).attr('id');

		confirm('Tem certeza que deseja desbloquear o pagamento deste aluno?');

		jQuery('#requisicao').val('desbloquearPagamento');
		jQuery('#estid').val(estid);
		jQuery('#form').submit();
	});
});

</script>

<form id="form" name="form" method="POST">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" value="" name="estid" id="estid" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita" width="5%">
				CPF:
			</td>
			<td align="left" width="13%">
				<? echo campo_texto('cpf', "N", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0, '', '', '', '' ); ?>
			</td>
			<td align="left">
				<input type="button" name="bloquear" class="bloquear" value="Inserir">
			</td>
		</tr>
	</table>
<?
	$sql = "SELECT DISTINCT
				'<center><img src=../imagens/alterar.gif border=0 width=13px
						  title=Desbloquear style=cursor:pointer;
						  id='|| est.estid ||' class=desbloquear /></center>',
				estcpf,
				estnome,
				CASE
					WHEN apc.apcesfera = 'M'
					THEN 'Municipal'
					ELSE 'Estadual'
				END as esfera,
				mun.estuf,
				mun.mundescricao,
				ent.entnome,
				tur.turdescricao
			FROM
				projovemcampo.estudante est
			INNER  JOIN projovemcampo.turma          tur ON tur.turid  = est.turid
			INNER  JOIN entidade.entidade 	         ent ON ent.entid  = tur.entid
			INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid  = tur.secaid
			INNER  JOIN entidade.endereco 	        ende ON ende.entid = tur.entid
			INNER JOIN territorios.municipio 	mun ON mun.muncod = ende.muncod
			WHERE
				bloqueadopagamento ='t'
			ORDER BY
				mun.estuf,
				mun.mundescricao,
				ent.entnome,
				tur.turdescricao
			";
	// ver($sql,d);
	monta_titulo('Alunos Bloqueados Para Receber Bolsa de Estudo', '');
	$cabecalho = array("Desbloquear","CPF","Nome","Esfera","Uf","Municipio","Escola","Turma");
	$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>
</form>
