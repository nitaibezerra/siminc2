<?php 
include_once(APPRAIZ."academico/classes/MaisMedicos.class.inc");
include_once(APPRAIZ."academico/classes/MaisMedicosResponsabilidade.class.inc");
echo '<br/>';

$mpeano = $_REQUEST['ano'] ? $_REQUEST['ano'] : date('Y');
$entid  = $_SESSION['academico']['entid'];
$orgid  = $_SESSION['academico']['orgid'];

monta_titulo( "Mais Médicos", "");

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
echo $autoriazacaoconcursos->cabecalho_entidade($entid);


$maisMedicos = new MaisMedicos();
$arrDados = $maisMedicos->recupaPorEntidade($entid);
if($arrDados){
	extract($arrDados);
}

?>
<style>.link{cursor:pointer}</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<form name="formulario" id="formulario" method="post" action="">
<input type="hidden" id="requisicao" name="requisicao" value="" />
<input type="hidden" id="classe" name="classe" value="" />
<input type="hidden" id="msminteresse" name="msminteresse" value=""" />
<input type="hidden" id="msmid" name="msmid" value="<?php echo $maisMedicos->msmid?>" />
<input type="hidden" id="entid" name="entid" value="<?php echo $entid?>" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="2" align="center" >
				<div style="width:90%;height:400px;border:solid 1px black;overflow:auto;padding:3px;background-color:#fff">
				<?php echo textoHTML($entid) ?>
				</div>
			</td>
		</tr>
</table>
</form>
<script>
function abreLinkAnexo(entid)
{
	return windowOpen('academico.php?modulo=principal/anexoPortaria&acao=A&entid='+entid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}
</script>