<script type="text/javascript">
	function postar(){
		//alert('teste');
		document.form.submit();
	}
</script>

<?php
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include APPRAIZ. '/includes/Agrupador.php';
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

$titulo = "PSE - Programa Saúde na Escola";
echo "<br>";
//monta_titulo( $titulo, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );
monta_titulo( $titulo, '<img border="0" src="../imagens/atencao.png"/> Ítens com pendência. <img border="0" src="../imagens/check_p.gif"/> Ítens OK. ' );

$entid = $_SESSION['pse']['entid'];
$muncod = $_SESSION['pse']['muncod'];
$tipo = $_SESSION['pse']['tipo'];

if($tipo==1 && !empty($entid)){
	
	
	$sql = "SELECT muncod, estuf FROM entidade.endereco WHERE entid = ".$entid;
	$arrDados = $db->pegaLinha( $sql );
	
	$ler = $tipo;
	$sql = "SELECT espid from pse.escolapse
			WHERE entid=$entid and espano = ".$_SESSION['exercicio'];
	$espid = $db->pegaUm($sql);
	if(!empty($espid))
		$_SESSION['pse']['espid'] = $espid;
	else {
		$sql = "INSERT INTO pse.escolapse
				(entid,espparticipapse2009, espano, muncod, estuf)
				values
				($entid,'f', ".$_SESSION['exercicio'].", '".$arrDados['muncod']."', '".$arrDados['estuf']."')
				RETURNING espid";
		$espid = $db->pegaUm($sql);		
		$_SESSION['pse']['espid'] = $espid;
		
	}
}
else if($tipo==2 && !empty($muncod)){
	$ler = $tipo;
	$sql = "select m.estuf, m.mundescricao
			from territorios.municipio m
			where m.muncod='$muncod'";
	$dados = $db->pegalinha($sql);
	$_SESSION['pse']['uf'] = $dados['estuf'];
	
	//Selecionar as secretarias em caso de capital
	$sql = "select count(*)
			from territorios.estado
			where muncodcapital='$muncod'";
	$quant = $db->pegaUm($sql);

		
}
else if($tipo==3 && !empty($entid)){
	$ler = $tipo;
	$sql = "SELECT espid from pse.escolapse
			WHERE entid=$entid and espano = ".$_SESSION['exercicio'];
	$espid = $db->pegaUm($sql);
	$_SESSION['pse']['espid'] = $espid;
}
else {
	echo "<script>window.location.href='pse.php?modulo=inicio&acao=C';</script>";
	exit;
}

?>

<style>
	#arvore img{
		border: none;
	}
</style>
<script language="javascript" type="text/javascript" src="/includes/dtree/dtree.js"></script>

<script type="text/javascript">
	function abrirJanela(caminho){
		window.location.href='/pse/pse.php?modulo=principal/' + caminho + '&acao=A';
	}
	function abrirJanela2(caminho){
		window.location.href='/pse/pse.php?modulo=relatorio/' + caminho + '&acao=A';
	}
	function abrirtermo(caminho){
		window.open('/pse/pse.php?modulo=principal/' + caminho + '&acao=A','','width=800,height=600,scrollbars=1');
	}
</script>


<?php
	$id = ($entid == '' ? $muncod : $entid);

	if($ler==2){
		if($pflcod=='SECRETARIA_ESTADUAL')
			$flag = 'e';
		elseif($pflcod=='SECRETARIA_MUNICIPAL')
			$flag = 'm';
			
		if ($quant>0 && !isset($_REQUEST['escolha'])){
			echo	"<form action='' method='post' name='form' id='form'>
						<table align='center' class='Tabela' cellpadding='2' cellspacing='1'>
					 		<tbody>
								<tr>
									<td width='100' class='SubTituloEsquerda'>
										<input type='radio' name='escolha' id='escolha' onclick='postar()' value='e'>Secretaria Estadual
										<input type='radio' name='escolha' id='escolha' onclick='postar()' value='m'>Secretaria Municipal
									</td>
								</tr>
							</tbody>";
			}
		else {
			$form = (!isset($_REQUEST['escolha'])?'m':$_REQUEST['escolha']);
			$flag = ($flag==''?$form:$flag);
			Cabecalho($muncod,$ler,$flag);
			//echo "pflcod: ".$pflcod."<br>varform: ".$form."<br>flag: ".$flag."<br>form: ".$_REQUEST['escolha'];
			echo	"<tr>
						<td style='padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;' colspan='2'>".
							montaArvore($ler,$id,$flag)."
						</td>
					</tr>";
		}
		
	}
	else {	
		Cabecalho($entid,$ler,$pflcod);
		echo	"<tr>
					<td style='padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;' colspan='2'>".
						montaArvore($ler,$id)."
					</td>
				</tr>";
	}	  
	 
?>

<tr>
	<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='2'>
		<input type="button" class="botao" name="btvoltar" value="Voltar" onclick="voltarpage()">
	</td>
</tr>
</table>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
function voltarpage(){
	var voltar = <?=$tipo;?>;
	switch (voltar){
		case 1 :
			caminho = 'ListarEscolas';
			break;
		case 2:
			caminho = 'listaMunicipios';
			break;
		case 3:
			caminho = 'ULILista';
			break;
	}
	window.location.href='pse.php?modulo=principal/'+caminho+'&acao=A';
}
</script>