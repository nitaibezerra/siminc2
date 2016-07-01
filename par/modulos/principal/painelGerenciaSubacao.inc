<?php
if(!$_REQUEST['modopopup']){
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Painel Gerencial', 'Visão geral dos municípios' );
} else {
?>
<style type="text/css">
 .titulo1{background-color:rgb(227, 227, 227);text-align:center;font-weight:bold;}
 .linha1{background-color:#f5f5f5;}
 .linha2{background-color:#fdfdfd;}
 </style>
 <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
 <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
 <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>

 <script type="text/javascript" src="../includes/funcoes.js"></script>
 <script type="text/javascript" src="../includes/remedial.js"></script>
 <script type="text/javascript" src="../includes/superTitle.js"></script>
 <script language="JavaScript" src="../includes/wz_tooltip.js"></script>
 
 <link rel="stylesheet" href="/includes/LyteBox/lytebox.css" type="text/css" media="screen" />
 <script type="text/javascript" src="/includes/LyteBox/lytebox.js" ></script>
 <?
} ?>
 
 <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
 <script type="text/javascript" src="../includes/remedial.js"></script>
 <script type="text/javascript" src="../includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form action="" method="POST" name="formulario">
	<input type="hidden" name="modopopup" value="<?=$_REQUEST['modopopup']?>">
	<?php if(!$_REQUEST['modopopup']){?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" style="width:29.5%;">Nome:</td>
			<td>
				<?
				$usunome = $_POST['usunome'];
				echo campo_texto( 'usunome', 'N', 'S', '', 100, 80, '', '', '','','','','' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Município:</td>
			<td>
				<?
				$mundescricao = $_POST['mundescricao'];
				echo campo_texto( 'mundescricao', 'N', 'S', '', 100, 80, '', '', '','','','','' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Perfil:</td>
			<td>
				<?
				$pflcod = $_POST['pflcod'];
				$arrPerfl = array(
								array('codigo' => PAR_PERFIL_EQUIPE_TECNICA, 	'descricao' => 'Analista Técnico'),
								array('codigo' => PAR_PERFIL_EQUIPE_FINANCEIRA, 'descricao' => 'Analista Financeiro')
								);
				
				$db->monta_combo( "pflcod", $arrPerfl, 'S', '--', '', '','','','','celid'); ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#dfdfdf" colspan=2 align="center">
				&nbsp;&nbsp;
				<input style="cursor:pointer;" type="button" name="btnPesquisar" id="btnPesquisar" value="Pesquisar"/>
				&nbsp;&nbsp;
				<input style="cursor:pointer;" type="button" name="btnLimpar" id="btnLimpar" value="Limpar"/>
				&nbsp;&nbsp;
				<input style="cursor:pointer;" type="button" name="btnPoupup" id="btnPoupup" value="Modo Popup"/>
			</td>
		</tr>
	</table> 
<?}
	    if($_POST){
	                
	        $filtro  = !empty($_POST['usunome']) ? " and removeacento(u.usunome) ilike '%{$_POST['usunome']}%'" : '';
	        $filtro .= !empty($_POST['mundescricao']) ? " and removeacento(m.mundescricao) ilike '%{$_POST['mundescricao']}%'" : '';
	        $filtro .= !empty($_POST['pflcod']) ? " and ur.pflcod = {$_POST['pflcod']}" : '';
	                 
			$sql = "SELECT DISTINCT
						u.usucpf,
						u.usunome
					FROM
						seguranca.usuario AS u
						inner join par.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
						left join territorios.municipio m on m.muncod = ur.muncod
						left join territorios.estado est on est.estuf = ur.estuf
					WHERE
						ur.rpustatus = 'A'
						$filtro
					ORDER BY u.usunome";				
			
			$listas = $db->carregar( $sql );
			$listas = $listas ? $listas : array();
			if( $listas ){
				echo '<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
				foreach($listas as $lts){
					$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;
					
					echo ($a%6) == 0 ? "<TR>" : "";
					
					if($lts['usunome']){
						$n = explode(' ', $lts['usunome']);
						$n1 = $n[0];
						$n2 = $n[count($n)-1];
					}
					
					$qtd = totalDemandasPorTec($lts['usucpf']);
					if($qtd == 0) $qtd = "<font color=red>($qtd)</font>";
					else $qtd = "<font color=blue>($qtd)</font>";	
					
					?><td valign='top' align="center" style="width: 133px">
								<table width="100%" class='tabela'> 
									<tr> 
										<th><font color='#00008B'><?=$n1 ." ". $n2 ." ". $qtd?></font></th> 
									</tr> 
									<tr> 
										<td valign='top' align="center" style="width: 133px">
											<div style="margin: 0 auto; padding: 0; height: 210px; width: 419px; background-color: #eee; border: none;" class="div_rolagem" id="finalizadas">
												<?=painelGerencia($lts['usucpf'])?>
											</div>
										</td>
									</tr>
								</table>
						   </td>
					<?
					echo ($a+1%6) == 0 ? "</TR>" : "";
					
					$a++;
					
				}
				echo '</table>';
			} else {
				print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
				print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
				print '</table>';
			}
	    }
?>
</form>
<script type="text/javascript">

	// Define delay do tooltip.
	//SetglobalIntFadeSpeed(100);
	
	$(document).ready(function(){
		$('#btnPesquisar').click(function(){
			if( $('[name="strperfil"]').val() == '' ){
				alert('O campo Perfil é obrigatório.');
				return false;
			}
			$('#modopopup').val();
			$('[name="formulario"]').submit();
		});
		
		$('#btnLimpar').click(function(){
			window.location.href = window.location;
		});
		
		$('#btnPoupup').click(function(){
			if( $('[name="strperfil"]').val() == '' ){
				alert('O campo Perfil é obrigatório.');
				return false;
			}
			$('#modopopup').val(1);
			$('[name="formulario"]').action = 'popPainelGerenciaSubacao.php';
			var janela = window.open( '', 'modopop', 'width=780,height=465,status=1,menubar=0,toolbar=0,scrollbars=1,resizable=1' );
			janela.focus();
			$('[name="formulario"]').target = 'modopop';
			$('[name="formulario"]').submit();
		});
	});
</script>

<?
function totalDemandasPorTec($usucpf){	
	global $db;
	
	//$and = " and doc.esdid in(91,107,92,108) ";
	
	$filtro  = !empty($_POST['mundescricao']) ? " and removeacento(mun.mundescricao) ilike '%{$_POST['mundescricao']}%'" : '';

	$sql = "SELECT count(*)
			FROM par.usuarioresponsabilidade ur 
				left join territorios.municipio mun on mun.muncod = ur.muncod
				left join territorios.estado 	est on est.estuf  = ur.estuf
			WHERE 
				ur.usucpf = '$usucpf'
				$filtro
			    and ur.rpustatus = 'A'";
			    
	$totalRegistro = $db->PegaUm( $sql );
	
	return $totalRegistro;
											
}

function painelGerencia($usucpf){
	
	global $db;
	
	//$and .= " and doc.esdid in(91,107,92,108) ";
	
    $filtro  = !empty($_POST['mundescricao']) ? " and removeacento(mun.mundescricao) ilike '%{$_POST['mundescricao']}%'" : '';

	$sql = "SELECT
				case when mun.mundescricao is not null then mun.mundescricao else est.estdescricao end as municipios,
			    '' as datanalise,
			    '' as qtdpendente,
			    '' as qtdanalisada
			FROM par.usuarioresponsabilidade ur 
				left join territorios.municipio mun on mun.muncod = ur.muncod
				left join territorios.estado 	est on est.estuf  = ur.estuf
			WHERE 
				ur.usucpf = '$usucpf'
				$filtro
			    and ur.rpustatus = 'A'";
				 
	if($sql) $RS = $db->carregar($sql);

	

?>


		<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>
			<tr>
				<td align='left' valign='top' width="40%" class='title' bgcolor='#dfdfdf' 
					style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;'><strong>Município</strong>
				<td align='left' valign='top' width="20%" class='title' bgcolor='#dfdfdf' 
					style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;'><strong>Data inicio Análise</strong>
				<td align='left' valign='top' width="20%" class='title' bgcolor='#dfdfdf' 
					style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;'><strong>Qtd Subações Pendentes</strong>
				<td align='left' valign='top' width="20%" class='title' bgcolor='#dfdfdf' 
					style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;'><strong>Qtd Subações Analisadas</strong>
			</tr>
			
				<? if ( $RS ) : ?>
				
					<?$cor = ''; ?>
					<?$ct = 0; ?>
					<?foreach ( $RS as $dado ) :					
						$cor = $cor == '#F7F7F7' ? '#ffffff' : '#F7F7F7' ;
						?>
						<tr bgcolor='<?=$cor ?>' onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor ?>';">
							<td ><?=$dado['municipios']?></td>
							<td ><?=$dado['datanalise']?></td>
							<td ><?=$dado['qtdpendente']?></td>
							<td ><?=$dado['qtdanalisada'] ?></td>
						</tr>
					<?endforeach;?>
					
				<?php else : ?>
					<tr>
						<td colspan=19 valign="middle" style='text-align:center; padding:80px 15px 80px 15px; background-color:#990000; color:#FFFFFF; font-weight:bold; font-size: 10px;' >
							Não existem municípios.
						</td>
					</tr>
				<?php endif; ?>
		</table>

<?php }?>