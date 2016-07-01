<?php
# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();

/*----------------- Configurações de Página ---------------------*/ 

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pdeid 	= $_SESSION["pdeid"];
$entid  = $_SESSION["entid"];
if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

$cDado = array("instrumento" => 'sintese_autoavaliacao');
$cForm = new formulario($cDado);
$cForm->direcPag('');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Plano de Ação ', '');
echo cabecalhoPDE(); 


if( $_POST['act'] == "salvar" )
{
	salvar();
}
 

function salvar()
{
	global $db;
 
	$perfis = arrayPerfil();
 
    if( ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis ) ) || ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis ) ) || ( in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis ) ) )
    { 
		if( $_POST['opcao'] )
		{
			$sqlDelete = "DELETE FROM pdeescola.justificativa WHERE pdeid = '{$_SESSION['pdeid']}'";
			$db->executar( $sqlDelete );
			$db->commit(); 
		} 
		for( $i = 0; $i <count($_POST['opcao']); $i++)
		{
			 
			$sql = "
				   INSERT INTO pdeescola.justificativa
				   (pdeid, opjid, jusdescricaooutros, jusanoreferencia)
				   VALUES
				   ('{$_SESSION['pdeid']}', 
				   {$_POST['opcao'][$i]},
				   '".substr($_POST['jusdescricaooutros_'.$i.''], 0, 1000)."', 
				   'now();'
				   )
				   ";
			if( $_POST['opcao'][$i] != ''){
				$insert = $db->executar( $sql );
			}
		}
		$db->commit();
    }
	else
    {
    	echo '<script type="text/javascript">alert("Você não tem permissão para inserir ou alterar dados nesta página");window.opener.location.reload(); window.close();</script>';  
    }
}

/*----------------- Recupera dados ---------------------*/  
$dados = $db->pegaLinha("SELECT 	pdeuf,
								pdemunicipio, 
								pdenome,
								pdenomediretor,
								pdebairro,
								pdelogradouro,
								pdecomplemento,
								pdetelefone,
								pdeemail,
								tloid
						FROM pdeescola.pdeescola
						WHERE pdeid = '".$pdeid."'");                                                   
extract($dados);
if($dados['tloid'] == 1){
	$check1 = 'checked="checked"';
}else if($dados['tloid'] == 2){
	$check2 = 'checked="checked"';
}else if($dados['tloid'] == 3){
	$check3 = 'checked="checked"';
}

$sql = "SELECT entnome as nome
		FROM entidade.entidade as e
		INNER JOIN entidade.funcaoentidade as fe ON fe.entid = e.entid
		WHERE funid = 3 and
		  	  tpcid IN (1,3) AND
	    	  e.entid IN ('{$entid}')";
$NomeEscola = $db->pegaum($sql);
$pdenome = $NomeEscola;

?>
 <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>


<?=subCabecalho(' Cadastrar Justificativa ');?>

<?php 
$sql = "select * from pdeescola.justificativa where pdeid = '{$_SESSION['pdeid']}'";
$rsDados = $db->carregar( $sql );
 
if( $_REQUEST['link'] == 'S')
{
	carregaJustificativas();
	$pos = 'pendentes';
}
else
{
	$pos = 'erros';
}

// pre($_SESSION[$pos]['programas_pendentes'], 1 );
for( $i = 0; $i <=count( $_SESSION[$pos]['programas_pendentes'] ) ; $i++)
{
 
	if( $_SESSION[$pos]['programas_pendentes'][$i] != '')
	{	
		$sqlProgramas = "SELECT 
							*
						 FROM pdeescola.tipoprograma 
						 WHERE
						 tprid = '{$_SESSION[$pos]['programas_pendentes'][$i]}'
						 ORDER BY
						 tprid";						 
		?>
		<form name="formulario" id="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post">		
		<?php				
		$rsProgramas = $db->carregar( $sqlProgramas );
		for( $j = 0; $j < count( $rsProgramas); $j++)
		{
			?>			
			<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
			<tr>
				<!-- <input type="hidden" name="tprid[<?=$j;?>]" id ="tprid[<?$j;?>]" value="<?=$rsProgramas[$j]['tprid']; ?>"> -->
				<td class="SubTituloEsquerda" bgcolor="#f5f5f5" colspan="2"><?php echo $rsProgramas[$j]['tprdescricao']; ?></td>
			</tr>
			<tr>
				<td>
					<?					
					//puxar os tipos de opção para cada um dos programas
					$sql = "SELECT opjid,trim(opjdescricao)as opjdescricao, substring(trim(opjdescricao), 0, 7) as compare FROM pdeescola.opcaojustificativa WHERE tprid = '{$rsProgramas[$j]['tprid']}' ORDER BY opjid";
					$rsOpcoes = $db->carregar( $sql );
					
					for( $k = 0; $k < count( $rsOpcoes); $k++ )
					{						
					 	$check1 = 'checked = "checked"'; 
					 	if( trim($rsOpcoes[$k]['compare']) == trim('Outros') )
					 	{
					 		 
					 		$onchange = "onclick=\"mostraOutros($i);\"";
					 	}
					 	else
					 	{
					 		$onchange = "onclick=\"escondeOutros($i);\"";
					 	}
						?>
						<input type="hidden" name="tamanho" id = "tamanho" value="<?=count( $rsOpcoes); ?>">
						<input type="radio" <?php  if( $rsDados[$i]['opjid'] == $rsOpcoes[$k]['opjid'] ) echo $check1; echo $onchange; ?> name="opcao[<?=$i;?>]" id="opcao[<?=$i;?>]" value="<?=$rsOpcoes[$k]['opjid']; ?>" ><?=$rsOpcoes[$k]['opjdescricao']; ?><br>
						<?
					}  
					if( $rsDados[$i]['jusdescricaooutros'] == '')
					{
						$style =  "style=\"visibility: hidden; position:absolute; display:inline;\"";
						$check2 = 'checked = "false"'; 
					}	
					else
					{
						 $style =  "style=\"visibility: visible; position:static; display:inline;\"";
						 $check2 = 'checked = "checked"';
					}
					?>
					<!-- 
					<input type="radio" <?php if( $rsDados[$i]['jusdescricaooutros'] != '') echo $check2; ?>  name="opcao[<?=$i;?>]" id="opcao[<?= $i;?>]" value='' onchange="mostraOutros(<?=$i;?>);" > Outros <br>
					-->
					<div id="outros[<?= $i;?>]" <?=$style; ?> valign="top">
						<? 
						$value = $rsDados[$i]['jusdescricaooutros'];
						$jusdescricaooutros = 'jusdescricaooutros'.'_'.$i; 
						?> 
						<div class="notprint">
							<textarea  id="<?=$jusdescricaooutros; ?>" name="<?=$jusdescricaooutros; ?>" cols="150" rows="8" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this );" style="width:150ex;" onkeydown="textCounter( this.form.<?=$jusdescricaooutros; ?>, this.form.no_<?=$jusdescricaooutros; ?>, 1000 );"  onkeyup="textCounter( this.form.<?=$jusdescricaooutros; ?>, this.form.no_<?=$jusdescricaooutros; ?>, 1000);" ><?= $value; ?></textarea> 
							<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
							<br>
								<input readonly style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_<?=$jusdescricaooutros; ?>" size="6" maxlength="6" value="1000" class="CampoEstilo">
								<font color="red" size="1" face="Verdana"> máximo de caracteres</font>
						</div>
						<div id="print_<?=$jusdescricaooutros; ?>" class="notscreen" style="text-align: left;">
						</div>             
	    	 		</div> 
	    	 			<?
//	    	 				echo 'o i aki vale:'. ($i+1);
//	    	 				echo '<br>';
//							echo $rsProgramas[$j]['tprid'];
	    	 			?>
	    	 			<input type="hidden" name="tipo_opjid[<?=$i;?>]" id = "tipo_opjid[<?=$i;?>]" value="<?=$rsProgramas[$j]['tprid'] ?>">
						<!-- 
						
						<?= campo_textarea( 'jusdescricaooutros'.'_'.$i, 'S', 'S', 'Descrição ', 150 , 8, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false ); ?>             
	    				 --> 
			
					
			
				</td>
			
			</tr>   
		  <?
		} 
	} 
} 
//limpando sessão
$_SESSION[$pos]['programas_pendentes'] == '';
?> 
			<tr>
			<input type="hidden" name="act" id = "act" value="0">
				<td colspan="2" align="center" class="SubTituloEsquerda">
				<?php
				//botão salvar só aparece se carregado pelo get na URL
				$perfis = arrayPerfil();
 
			    if( ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis ) ) || ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis ) ) || ( in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis ) ) )
			    { 	
	 			?>
					<input type="button" name="btnSalvar" id="btnSalvar" value="Salvar" onclick="salvar();" >
 				<?
 				}
 				?>
				</td>
			</tr>
			</table>
			</form>
<script> 

function salvar()
{	
	if( top.outros_0 == 't' )
	{
		var campotexto = document.getElementById('jusdescricaooutros_0');
		if( campotexto.value == '' )
		{
			alert('É preciso descrever uma justificativa caso selecione a opção "Outros"');
			return false;
		}
	} 
	if( top.outros_1 == 't' )
	{
		var campotexto = document.getElementById('jusdescricaooutros_1');
		if( campotexto.value == '' )
		{
			alert('É preciso descrever uma justificativa caso selecione a opção "Outros"');
			return false;
		}
	} 
	document.formulario.act.value = "salvar";
	document.formulario.submit();
}


function mostraOutros(id)
{	 
	if(id == 0)
	{ 
		var arr_0 = 't';
		top.outros_0 = arr_0;
	}
	else if(id == 1)
	{
		var arr_1 = 't';
		top.outros_1 = arr_1;
	} 
	 
	top.outros = 'true';
	document.getElementById('outros['+id+']').style.visibility= "visible";
	document.getElementById('outros['+id+']').style.position= "static";
}

function escondeOutros(id)
{	
	if(id == 0)
	{ 
		var arr_0 = 'f';
		top.outros_0 = arr_0;
	}
	else if(id == 1)
	{
		var arr_1 = 'f';
		top.outros_1 = arr_1;
	}
	 
	top.outros = 'false';
	document.getElementById('outros['+id+']').style.visibility= "hidden";
	document.getElementById('outros['+id+']').style.position= "absolute";
	document.getElementById('jusdescricaooutros_'+id).value = '';
}


</script>
