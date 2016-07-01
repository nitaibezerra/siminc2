<?php
#arquivo popupCadAtivaConvenio.inc
/*
echo("<pre>");
print_r($_POST);
echo("</pre>");
*/
//die();

include APPRAIZ . 'includes/Agrupador.php';
//$db->executar("SET CLIENT_ENCODING TO 'ISO-8859-1'");

//aqui dentro o cï¿½digo para inserï¿½ao, alteraï¿½ao e exclusao

if (array_key_exists('alterar', $_REQUEST ) ) 
{
	$sql = "SELECT cf.covnumero, ci.coistatus, ci.coitipo, ci.covcod 
			FROM parindigena.convenioindigena AS ci 
			INNER JOIN financeiro.convenio AS cf ON trim(cf.covcod) = trim(ci.covcod)
			WHERE trim(ci.covcod) = trim('{$_REQUEST['alterar']}') ";
			
	$ra = $db->carregar( $sql ); 
	
}

if ($_REQUEST['act'] == 'inserir')
{
	//echo("<script>alert('vai inserir');\n</script>");
	$covcod    = $_POST['covcod'];
	$coitipo   = 'F';
	$coistatus = $_POST['coisituacao'];
	$covnumero = $_POST['covnumero'];
		
	if( !array_key_exists('alterar', $_REQUEST ))
	{
		$sqlVerifica = "SELECT covcod FROM financeiro.convenio WHERE covnumero = '$covnumero'";
		$existe = $db->pegaUm( $sqlVerifica );
		
		if( $existe )
		{
			
			if (!$covcod)
				$covcod = $existe;			
			
			$sql = sprintf("SELECT 
								covcod 
							FROM parindigena.convenioindigena
			 				WHERE 
			 					coistatus = 'A'
			 					AND covcod = '%s'",
							$covcod
						);
			//echo $sql;
			//die;
			
			$convenio = $db->carregar($sql);
			$convenio = $convenio ? $convenio : array();
			
			if (count($convenio)){?>
				<script language="Javascript">
					alert('Este convênio já está cadastrado no sistema.');
					history.back(-1);
					//return;
				</script>
				<?
				return false;
			}
			
			//***************************************************************************
			$sql = sprintf("SELECT 
								covcod 
							FROM parindigena.convenioindigena
			 				WHERE
			 					covcod = '%s'",
							$covcod
						);
			
			$convenioAlteracao = $db->carregar($sql);
			$convenioAlteracao = $convenioAlteracao ? $convenioAlteracao : array();
			//****************************************************************************
			
			if (count($convenioAlteracao)){
				$sql = "UPDATE parindigena.convenioindigena 
						SET coitipo = '{$coitipo}',
							coistatus = '{$coistatus}'
						WHERE
							covcod = '{$covcod}'";
			}else{
				$sql = "INSERT INTO parindigena.convenioindigena (covcod, coitipo, coistatus) VALUES ('$covcod','$coitipo', '$coistatus')";
			}
			
			//echo $sql;
			//die;
			
			if ($insert = $db->executar( $sql ) )
			{
				$db->commit( $sql );
				echo("<script>alert('Operação realizada com sucesso.')\n</script>");				
			}
			else
			{
				echo("<script>alert('Número de convênio inválido.')\n</script>");
			}
		}
		else
		{
			echo("<script>alert('Número de convênio inválido.')\n</script>");
		}
	}
	else
	{
		$sql = "UPDATE parindigena.convenioindigena 
				SET coitipo = 'F',
					coistatus = '{$_REQUEST['coisituacao']}'
				WHERE
					covcod = '{$_REQUEST['covcod']}'";
			
	if ( $db->executar( $sql ) )
		{
			$db->commit( $sql );
			echo("<script>alert('Operação realizada com sucesso.')\n</script>");
			echo("<script>window.location.href = 'parindigena.php?modulo=principal/popupCadAtivaConvenio&acao=I'</script>");
			
		}
		else
		{
			echo("<script>alert('Não foi possivel a alteração dos dados do convênio.')\n</script>");
		}
	}
	unset($_POST['covcod']);
    unset($_POST['act']);

}

if (array_key_exists('excluir', $_REQUEST ) ) 
{

	//$sql = "UPDATE parindigena.convenioindigena SET coistatus = 'I' WHERE trim(covcod) = trim('{$_REQUEST['excluir']}')";
	$sql = "UPDATE parindigena.convenioindigena SET coistatus = 'I' WHERE covcod = '{$_REQUEST['excluir']}'";

	$db->executar( $sql );
	$db->commit();
	echo("<script>window.location.href = 'parindigena.php?modulo=principal/popupCadAtivaConvenio&acao=I'</script>");
}


include APPRAIZ . 'includes/cabecalho.inc';
?>
<br>   <script type="text/javascript" src="/includes/prototype.js">

</script>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Vincular convênio ao monitoramento</label></td></tr><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td></tr></table>
 
    <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         
            <tr>
                <td class ="SubTituloDireita" align="right">Número do Convênio: </td>
                <td> 
                     <?php 
                                   
				if ( $_REQUEST['act'] == 'busca' )
				{
					$covnumero = $_POST['covnumero'];
					$sql = "SELECT * FROM financeiro.convenio WHERE covnumero LIKE '$covnumero%' limit 1";
 
					$rs = $db->carregar( $sql );
				}
				   
                    $covnumero =  $ra[0]['covnumero']; 
                    ?>
             <?= campo_texto('covnumero', 'S', $somenteLeitura, '', 30, 600, '', '', 'left', '',  0, 'id="covnumero" onblur="MouseBlur(this);"' ); ?>
              <img src="/imagens/pop_p.gif" onclick="abrirPopupConvenio();" title="Pesquisar Convênios">
              
              <input type=hidden name="act" id = "act" value="0">
	         
                </td>
                <td align="left">
           
                </td>
                <input type="hidden" name="covcod"  value="<?php echo $ra[0]['covcod']; ?>">
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Situação: </td>
                <td>  
                    <select name="coisituacao" >
                   		<option value=''>Selecione...</option>
	                    <option value="A" <? if($ra[0]['coistatus'] == 'A') echo("selected = selected"); ?>>Ativo</option>
	                    <option value="I" <? if($ra[0]['coistatus'] == 'O') echo("selected = selected"); ?>>Inativo</option>
                    </select> 
                    <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
                </td>
                <td align="left">  
                     
                   
                </td>
                </tr>
            <tr>
            	<td  class ="SubTituloDireita" align="right">
            	
            	 </td>
            	 <td>
            	  <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="validaForm('grava');">  
            	  </td>
            	  <td>
            	  </td>
            </tr>	
		
          </form> 
          <tr>
            <td class="SubTituloDireita" colspan = "3">
                <?php
                
                                                  
            	$sql = "SELECT
                        	'<img onclick=\"return alterarConvenioIndigena('|| ci.covcod ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirConvenioIndigena('|| ci.covcod ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                            cf.estuf,
                        	cf.covnumero,
                            cf.covvalor, 
                            	CASE 
                                   	WHEN ci.coitipo = 'F' THEN 'Formação'
						           	ELSE '...'
						           END 
                        FROM 
                        	parindigena.convenioindigena AS ci
                        INNER JOIN 
							financeiro.convenio AS cf ON ci.covcod = cf.covcod
						WHERE ci.coistatus = 'A' AND ci.coitipo = 'F'
						ORDER BY
							cf.estuf,
                        	cf.covnumero
                       ";
                                      
  
                $cabecalho = array( "Opções","UF", "Número ", "Valor do Convênio (R$)", "Tipo do Convênio" );
                
                  
              	$db->monta_lista($sql, $cabecalho, 20, 10, 'N', 'left', '' );
                
                ?>
            </td>
           </tr>
           <tr>
           	<td>
           	
       </table>
       
 <script type="text/javascript">

/* Funï¿½ao para validar os dados do formulario requisitante.
 * author: PedroDantas
 * date: 2008-08-25
 */
function validaForm(type)
{  
	 
	var type;
	if( type == 'grava' )
	{
		var covnumero = document.formulario.covnumero.value;
		if( covnumero == '' ) 
		{
			alert('É necessário informar um número de convênio.');
			return false;
		}
//		var coitipo = document.formulario.coitipo.value;
//		if( coitipo == '' ) 
//		{
//			alert('É necessário informar o tipo de convênio.');
//			return false;
//		}
		var coisituacao = document.formulario.coisituacao.value;
		if( coisituacao == '' ) 
		{
			alert('É necessário informar uma situação para o convênio.');
			return false;
		}
		document.formulario.act.value = "inserir";
    }
    else if( type == 'busca' )
	{
		var covnumero = document.formulario.covnumero.value;
		if( covnumero == '' ) 
		{
			alert('Para efetuar a busca, digite um número de convênio.');
			return false;
		}
		//alert( type);	
		document.formulario.act.value = "busca";
    }
    document.formulario.submit();
	 

}
 
 /* Funï¿½ao para excluir uma unidade de medida
 * author: PedroDantas
 * date: 2008-08-25
 * params: undid (id da unidade selecionada)
 */
 function excluirConvenioIndigena(covcod)
         {
             if (!confirm('Deseja excluir o registro?')) 
             {
             	return false;
                
             }
              return window.location.href = 'parindigena.php?modulo=principal/popupCadAtivaConvenio&acao=I&excluir='+covcod;
                
             
         }
         
function alterarConvenioIndigena(covcod)
{    
    return window.location.href = 'parindigena.php?modulo=principal/popupCadAtivaConvenio&acao=I&alterar=' + covcod;
 }
 
function abrirPopupConvenio()
{
    return windowOpen('parindigena.php?modulo=principal/popupPesquisaConvenio&acao=I',
                  'popupPesquisaConvenio',
                  'height=560,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
} 
 
 
 </script>

 
 
 
<?php
$db->executar("RESET CLIENT_ENCODING");


?>