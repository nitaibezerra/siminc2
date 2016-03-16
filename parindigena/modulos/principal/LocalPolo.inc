<?php

require_once APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
   
//aqui dentro o código para inserçao, alteraçao e exclusao
if ($_REQUEST['alterar']) 
{
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
        $sql = "SELECT 
                    l.locid 
                FROM 
                    parindigena.localpolo AS l
                LEFT JOIN
                    parindigena.formacaocontinuada AS pf
                        ON l.locid = pf.locid
                LEFT JOIN
                    parindigena.formacaoinicialetapa AS pe
                        ON l.locid = pe.locid
                WHERE 
                    l.locid = {$_REQUEST['excluir']}";
              
         $numRegistros = $db->carregar($sql);       
                  
         $num = count($numRegistros);        
         
         //se o count do array do resultado for maior que 0, entao nao sera possivel a exclusao!.
         if ($num > 1 ) 
         {                         
             echo("<script>alert('Nao é possivel a exclusao deste Local/Pólo, pois o mesmo possui um ou mais vínculos');\n</script>");
             $var = 1;   
         }
         else
         { 
          
             $sql = "DELETE FROM parindigena.localpolo WHERE locid = {$_GET['excluir']}";
            
             $db->executar( $sql );
             $db->commit();
             
             header('Location: parindigena.php?modulo=principal/LocalPolo&acao=I');
          }
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
    {    //faz o insert ou update
        
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
                            
                            alert('Dados alterados com sucesso!');
                            
                            //header('Location: parindigena.php?modulo=principal/LocalPolo&acao=I');
        } 
        else         
        {                                                      
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
                                                                                  
                                                                                   
            $objResultado = $db->executar($sql);                                                   
            $db->commit();     
            
           $arrLinha = pg_fetch_assoc($objResultado);
           $intArquivoId = $arrLinha[ 'endid' ];           
           
            $sql = "INSERT INTO 
                         parindigena.localpolo (locnome, endid) 
                    VALUES ( 
                              '{$_REQUEST['locnome']}',
                               {$intArquivoId}
                           )";                            
                                                                     
            $db->executar($sql);
            $db->commit();

            alert('Dados gravados com sucesso!');   			
                            
		}        
	}      
}
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>
<script src="/includes/prototype.js"></script>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" align="center">
			<label class="TituloTela" style="color:#000000;">Local / Pólos</label>
		</td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" >
		</td>
	</tr>
</table>    
<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadLocalPolo" name="cadLocalPolo" onsubmit="return validaForm()">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">         
		<tr>
			<td class ="SubTituloDireita" align="right"> Nome do Local / Pólos: </td>
			<td> 
				<?php $locnome = $rsDadosNivel[0]["locnome"]; ?>
				<?= campo_texto('locnome', 'S', $somenteLeitura, '', 100, 600, '', '', 'left', '',  0, 'id="locnome" onblur="MouseBlur(this);"' ); ?>
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
				<input type="hidden" name="gravaForm" value="true">
                <?= campo_texto('endereco[endcep]', 'N', $somenteLeitura, '', 13, 10, '##.###-###', '', 'left', '',  0, 'id="enderecoCEP"', '', $entidade->enderecos[0]->endcep, 'Entidade.__getEnderecoPeloCEP(this)') ?>                
			</td>
		</tr>
		<tr id="escolha_logradouro_id" style="display:none">
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Selecione o Logradouro:</label></td>
			<td>	             
				<input readonly="readonly" type="text" name="endlog" class="CampoEstilo" id="endlog" value="<?=$entidade->enderecos[0]->endlog?>" size="48" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Logradouro:</label></td>
			<td>
				<?= campo_texto('endereco[endlog]', 'N', $somenteLeitura, '', 48, '', '', '', 'left', '',  0, 'id="endlogradouro" onblur="Entidade.__getEnderecoPeloCEP(this), MouseBlur(this);"', '', $entidade->enderecos[0]->endlog); ?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>
			<td>	                
				<?= campo_texto('endereco[endnum]', 'N', $somenteLeitura, '', 5, 8, '', '', 'left', '',  0, 'id="endnum" onblur="Entidade.__getEnderecoPeloCEP(this), MouseBlur(this);"', '', $entidade->enderecos[0]->endnum); ?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
			<td>            
                <?= campo_texto('endereco[endcom]', 'N', $somenteLeitura, '', 48, 100, '', '', 'left', '',  0, 'id="endcom" onblur="Entidade.__getEnderecoPeloCEP(this), MouseBlur(this);"', '', $entidade->enderecos[0]->endcom); ?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
			<td>            
                <?= campo_texto('endereco[endbai]', 'N', $somenteLeitura, '', 60, '', '', '', 'left', '',  0, 'id="endbai" onblur="Entidade.__getEnderecoPeloCEP(this), MouseBlur(this);"', '', $entidade->enderecos[0]->endbai); ?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>
			<td>            
                <?= campo_texto('mundescricao', 'N', $somenteLeitura, '', 40, '', '', '', 'left', '',  0, 'id="mundescricao" onblur="Entidade.__getEnderecoPeloCEP(this), MouseBlur(this);"', '', $entidade->enderecos[0]->getMunDescricao()) ?>
                <input type="hidden" name="endereco[muncod]" id="muncod" class="CampoEstilo" value="<?=$entidade->enderecos[0]->muncod?> " />
                <input type="hidden" name="endereco[endid]" id="endid" class="CampoEstilo" value="<?=$entidade->chavePrimaria[0]?>" />                
                <?= campo_texto('endereco[estuf]', 'N', $somenteLeitura, '', 4, 2, '', '', 'left', '',  0, 'id="estuf" onblur="Entidade.__getEnderecoPeloCEP(this), MouseBlur(this);"', '', $entidade->enderecos[0]->estuf) ?>
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
				<input type="submit" name="btnBuscar" value="Buscar" />
				<input type="submit" name="btnGravar" value="Salvar" />                     
			</td>
		</tr>
	</table>
</form>
<?php   
             
if (array_key_exists('btnBuscar', $_REQUEST))
{
	$sql = "SELECT
				'<img onclick=\"return alterarLocalPolo('|| locid ||',' ||endid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirLocalPolo('|| locid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
				locid, 
				locnome
			FROM 
				parindigena.localpolo";
                                     
	if( strtoupper($_REQUEST['locnome']))
	{     
		$sql.=" WHERE 
					UPPER(locnome)  ILIKE '%" .strtoupper($_REQUEST['locnome']). "%' ";
	}
                   
	if( strtoupper($_REQUEST['locnome']))
	{     
		$sql.=" ORDER BY
					locnome ";
	}                                    
		$buscar = 1;
}
else
{
	$sql = "SELECT
				'<img onclick=\"return alterarLocalPolo('|| locid ||',' ||endid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirLocalPolo('|| locid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
				locid, 
				locnome
			FROM 
				parindigena.localpolo";  
}
                
$cabecalho = array( "Opções", "Código ", "Local/Pólo" );        
$db->monta_lista($sql, $cabecalho, 20, 10, 'N', '', '' );
                
?>       
<script type="text/javascript">

function validaForm()
{      
	d = document.cadLocalPolo;
	cep = document.getElementById('enderecoCEP')
    
	if (d.locnome.value == '')
	{
		alert('O campo NOME DO LOCAL é obrigatório.');
		d.locnome.onfocus();    	
		return false;
	}           
} 

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