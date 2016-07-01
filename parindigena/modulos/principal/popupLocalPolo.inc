<?php

include APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
   
//aqui dentro o código para inserçao, alteraçao e exclusao
if ($_REQUEST['alterar']) {
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT
                *
            FROM 
                parindigena.localpolo 
            WHERE
                locid = {$_REQUEST['alterar']}";               
    
    $rsDadosNivel = $db->carregar( $sql );
}

if ($_REQUEST['excluir'])
{     
	$sql = "DELETE FROM parindigena.localpolo WHERE locid = {$_GET['excluir']}";            
	$db->executar( $sql );
	$db->commit();
             
	header('Location: parindigena.php?modulo=principal/LocalPolo&acao=I');
 
	if ($var == 1)
	{              
		echo '<script type="text/javascript">window.location.href = "parindigena.php?modulo=principal/LocalPolo&acao=I";</script>';
	} 
}

if (array_key_exists('btnGravar', $_REQUEST))
{
        $endereco = new Endereco($_REQUEST['endid']);
        $entidade->enderecos[0] = $endereco;
        
        $locnome              = $_REQUEST['locnome'];
        $endcep               = str_replace(array(".","-"),'',$_REQUEST['endereco']['endcep']);
        $endlog               = $_REQUEST['endereco']['endlog'];
        $endcom               = $_REQUEST['endereco']['endcom'];
        $endbai               = $_REQUEST['endereco']['endbai'];
        $muncod               = $_REQUEST['endereco']['muncod'];
        $estuf                = $_REQUEST['endereco']['estuf'];
        $endnum               = $_REQUEST['endereco']['endnum'];    
        $endstatus            = $_REQUEST['endereco']['endstatus'];  
        
        $medlatitude  = $_REQUEST['graulatitude'] .'.'. $_REQUEST['minlatitude'] .'.'. $_REQUEST['seglatitude'] .'.'. $_REQUEST['pololatitude'];
        $medlongitude = $_REQUEST['graulongitude'] .'.'. $_REQUEST['minlongitude'] .'.'. $_REQUEST['seglongitude'];
 
    if($_REQUEST['locnome']!='')
    {     
         if($_REQUEST['alterar'])
         {
                 $sql = "UPDATE 
                            parindigena.localpolo 
                         SET 
                            locnome = '{$_REQUEST['locnome']}'
                         WHERE 
                            locid = '{$_REQUEST['alterar']}'";
                 
                            $db->carregar($sql);
                                                       
                $sql = "UPDATE 
                            entidade.endereco 
                         SET 
                            endcep = '$endcep', 
                            endlog = '$endlog', 
                            endcom = '$endcom', 
                            endbai = '$endbai', 
                            muncod = '$muncod',  
                            estuf  = '$estuf', 
                            endnum = '$endnum', 
                            endstatus    = '$endstatus',  
                            medlatitude  = '$medlatitude', 
                            medlongitude = '$medlongitude'
                         WHERE 
                            endid = '{$_REQUEST['endid']}'";
                 
                            $db->carregar($sql);
                            $db->commit();
                            
                            header('Location: parindigena.php?modulo=principal/LocalPolo&acao=I');
        }
        else {                                                      
            $sql = "INSERT INTO                                                   
                          entidade.endereco 
                             (
                              endcep, 
                              endlog, 
                              endcom, 
                              endbai, 
                              muncod,  
                              estuf, 
                              endnum, 
                              endstatus,  
                              medlatitude, 
                              medlongitude
                             )       
                    VALUES 
                            (                                                   
                               '$endcep', 
                               '$endlog', 
                               '$endcom', 
                               '$endbai', 
                               '$muncod',  
                               '$estuf', 
                               '$endnum', 
                               '$endstatus',  
                               '$medlatitude',
                               '$medlongitude'
                            ) 
                            RETURNING endid
                            ";                                                   
                                                                 
            $objResultado = $db->pegaUm($sql);                                                   
                 
            $sql = "INSERT INTO 
                         parindigena.localpolo (locnome, endid) 
                    VALUES ( 
                              '{$_REQUEST['locnome']}',
                               {$objResultado}
                           ) returning locid";
                                    
            $locid = $db->pegaUm($sql);
            $db->commit(); 
         }
            echo '
            <script type="text/javascript" src="/includes/prototype.js"></script>
			<script type="text/javascript">
				window.opener.location.reload();
				window.close();
			</script>';
            die(); 
        }
   }  
?> 
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="-1">
<title><?php echo $titulo ?></title>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<style type="text/css" media="screen">
	#loader-container,
	#LOADER-CONTAINER {
		background: none;
		position: absolute;
		width: 100%;
		text-align: center;
		z-index: 8000;
		height: 100%;
	}

	#loader {
		background-color: #fff;
		color: #000033;
		width: 300px;
		border: 2px solid #cccccc;
		font-size: 12px;
		padding: 25px;
		font-weight: bold;
		margin: 150px auto;
	}
</style>
<script type="text/javascript">
	
	function validaForm()
	{
		d = document.cadLocalPolo;
		
		if (d.locnome.value == '')
		{
			alert('O campo NOME DO LOCAL é obrgatório!');
			d.locnome.focus();
			return false;
		}
		
		if (d.endcep.value == '')
		{
			alert('O campo CEP é obrgatório!');
			d.endcep.focus();
			return false;
		}
		
		d.submit();
		alert('Cadastro efetuado com sucesso.');	
		   
	}
	 
	/* Funçao para excluir uma unidade de medida
	 * author: PedroDantas
	 * date: 2008-08-25
	 * params: undid (id da unidade selecionada)
	 */
	function excluirLocalPolo(locid)
	{
		if (confirm('Deseja excluir o registro?')) 
		{
			return window.location.href = 'parindigena.php?modulo=principal/LocalPolo&acao=I&excluir=' + locid;                
		}
	             
	}
	         
	function alterarLocalPolo(locid, endid)
	{    
		return window.location.href = 'parindigena.php?modulo=principal/LocalPolo&acao=I&alterar=' + locid + '&endid=' + endid;
	}  
</script> 
</head>
<body>
	<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
		<tr>
			<td width="100%" align="center">
				<label class="TituloTela" style="color:#000000;">Local / Pólo</label>
			</td>
		</tr>
		<tr>
			<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td>
		</tr>
	</table>    
	<form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadLocalPolo" name="cadLocalPolo">
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"> 
			<tr>
				<td class ="SubTituloDireita" align="right"> Nome do Local: </td>
				<td> 
					<?php $locnome = $rsDadosNivel[0]["locnome"];?>
					<?= campo_texto('locnome', 'S', $somenteLeitura, '', 80, 600, '', '', 'left', '',  0, 'id="locnome" onblur="MouseBlur(this);"' ); ?>
				</td>
				<td align="left"></td>
			</tr>
			<?php 
	        $endereco = new Endereco($_REQUEST['endid']);
			$entidade->enderecos[0] = $endereco;        
			?>                
			<tr>
				<td style="font-weight: bold" colspan="2">Endereço</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>
				<td>					
					<?= campo_texto('endereco[endcep]', 'S', $somenteLeitura, '', 13, 10, '##.###-###', '', 'left', '',  0, 'id="endcep"','',$entidade->enderecos[0]->endcep,'Entidade.__getEnderecoPeloCEP(this)' ); ?>
				</td>
			</tr>
			<tr id="escolha_logradouro_id" style="display:none">
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Selecione o Logradouro:</label>
				</td>
				<td>					
					<?= campo_texto('endlog', 'N', $somenteLeitura, '', 48, '', '', '', 'left', '',  0, 'id="endlog" onblur="MouseBlur(this);"','',$entidade->enderecos[0]->endlog ); ?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Logradouro:</label>
				</td>
				<td>					
					<?= campo_texto('endereco[endlog]', 'N', $somenteLeitura, '', 48, '', '', '', 'left', '',  0, 'id="endlogradouro"','',$entidade->enderecos[0]->endlog ); ?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Número:</label>
				</td>
				<td>					
					<?= campo_texto('endereco[endnum]', 'N', $somenteLeitura, '', 5, 8, '', '', 'left', '',  0, 'id="endnum"','',$entidade->enderecos[0]->endnum ); ?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Complemento:</label>
				</td>
				<td>					
					<?= campo_texto('endereco[endcom]', 'N', $somenteLeitura, '', 48, 100, '', '', 'left', '',  0, 'id="endcom"','',$entidade->enderecos[0]->endcom ); ?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Bairro:</label>
				</td>
				<td>					
					<?= campo_texto('endereco[endbai]', 'N', $somenteLeitura, '', 35, '', '', '', 'left', '',  0, 'id="endbai"','',$entidade->enderecos[0]->endbai ); ?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Município/UF: </label>
				</td>
				<td>	                
	                <?= campo_texto('mundescricao', 'N', $somenteLeitura, '', 25, '', '', '', 'left', '',  0, 'id="mundescricao"','',$entidade->enderecos[0]->getMunDescricao() ); ?>
	                <input type="hidden" name="endereco[muncod]" id="muncod" class="CampoEstilo" value="' . $entidade->enderecos[0]->muncod " />
	                <input type="hidden" name="endereco[endid]" id="endid" class="CampoEstilo" value="' .  $entidade->chavePrimaria[0] " />	                
	                <?= campo_texto('endereco[estuf]', 'N', $somenteLeitura, '', 4, '', '', '', 'left', '',  0, 'id="estuf" style="width: 5ex; padding-left: 2px"','',$entidade->enderecos[0]->estuf ); ?>
				</td>
			</tr>                
			<tr> 
				<td style="font-weight: bold" colspan="2">Coordenadas Geográficas</td>
			</tr>  
			<tr>
				<td class ="SubTituloDireita" align="right"> Latitude: </td>
				<td>
					<?php 
						$medlatitude = $entidade->enderecos[0]->medlatitude;
						$latitude = explode(".", $medlatitude);
						$graulatitude = $latitude[0];
						$minlatitude  = $latitude[1];
						$seglatitude  = $latitude[2];
						$pololatitude = $latitude[3];
					?>
					<?= campo_texto( 'graulatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?> °
					<?= campo_texto( 'minlatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?> '
					<?= campo_texto( 'seglatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?> ''
					<?= campo_texto( 'pololatitude', 'N', $somenteLeitura, '', 1, 1, '', '', 'left', '', 0, 'id="pololatitude"'); ?> (N/S)
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right"> Longitude: </td>
				<td>
					<?php 
					    $medlongitude = $entidade->enderecos[0]->medlongitude;
						$longitude = explode(".", $medlongitude);
						$graulongitude = $longitude[0];
						$minlongitude  = $longitude[1];
						$seglongitude  = $longitude[2];
					?>
					<?= campo_texto( 'graulongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?> °
					<?= campo_texto( 'minlongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?> '
					<?= campo_texto( 'seglongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"'); ?> '' &nbsp W
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right" colspan="3"> 
					<input type="submit" name="btnGravar" value="Salvar" />
				</td>
			</tr>		
		</table>
	</form> 
</body>