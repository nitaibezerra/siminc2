<?
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include APPRAIZ . "includes/classes/dateTime.inc";
VerSessao();
$entid = $_SESSION['pse']['entid'];
$espid = $_SESSION['pse']['espid'];
	
if($_POST){
	if($_REQUEST['destino']==''){
		echo "<script>alert('É necessário, pelo menos, um ator!');
						history.go(-1);
				</script>";
	}
	else {
	$sql = "DELETE FROM pse.atorequipe
			WHERE espid = $espid";
	$db->executar($sql);
	
	foreach( $_REQUEST['destino'] as $ator ){
		$sql = "INSERT INTO pse.atorequipe
				(espid, atrid)
				VALUES
				($espid, $ator)";
		$db->executar($sql);		
	}
	$db->commit();
	echo "<script>alert('Operação realizada com sucesso!');</script>";
	}
}
	
include APPRAIZ . 'includes/Agrupador.php'; 
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$titulo = "PSE - Programa Saúde na Escola";
monta_titulo( $titulo, '' );
$titulo = "Atores PSE";
monta_titulo( $titulo, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/agrupador.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>

<form method="POST"  name="formulario" onSubmit="selectAllOptions( document.getElementById('destino'))">
<?php Cabecalho($entid,1); ?>

<tr>
	<td colspan="2" style="text-align: left;"><b>3. Atores que integram a equipe do PSE na Escola.</b></td>
</tr>
<tr>
    <td colspan="2"  style="background: rgb(238, 238, 238)">
    <?
		$sql = "SELECT atrid as codigo, atrdsc as descricao
				FROM pse.ator
				ORDER BY atrdsc";
		$dados = $db->Carregar($sql);
		
		$sql = "SELECT b.atrid as codigo, a.atrdsc as descricao
				FROM pse.ator a
				INNER JOIN
					pse.atorequipe b ON b.atrid = a.atrid
				WHERE b.espid=$espid
				ORDER BY atrdsc";
		$dados2 = $db->Carregar($sql);
		
		$Atores = new Agrupador( 'formulario' );
		$Atores->setOrigem( 'origem', null, $dados );
		$Atores->setDestino( 'destino', null, $dados2);
		$Atores->exibir();
	?>
    </td>
</tr> 
<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
<?php 
	$sql = "SELECT * FROM pse.programacaoexercicio WHERE prsano = '".$_SESSION['exercicio']."'";
	$arr = $db->pegaLinha( $sql );
	$dataAtual = date("Y-m-d");
	$data = new Data();
	$dataF = trim($arr['prsdata_termino']);
	$resp = 1;
		if( !empty($dataF) ){
			$resp = $data->diferencaEntreDatas($dataAtual, $arr['prsdata_termino'], 'maiorDataBolean','','');		
		}
	if( $resp == NULL ){ ?>
		<tr bgcolor="#cccccc">
			<td style="text-align: center" colspan='2'>
				<input type="submit" class="botao" name="btsalvar" value="Salvar">
			</td>
		</tr>
	<?php } ?>
<?php } ?> 
<?=navegacao('AtendimentoESF','cadastroEstadoMunicipioArvore');?>
</table>
</form>


<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>