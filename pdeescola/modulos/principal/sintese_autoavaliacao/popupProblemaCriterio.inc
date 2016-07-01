<?php
# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();

#popupProblemaCriterio
function verificaTipoPerfil( $usucpf )
{    
    global $db;
    $sql = "SELECT pflcod FROM seguranca.perfilusuario WHERE usucpf = '$usucpf'";
    $perfil = $db->pegaUm( $sql );
    return $perfil;    
}

if( $_REQUEST['tipo'] == 'problema' )
{
    $titulo    = "Cadastro de Problemas";
    $subtitulo = "Descrição do Problema";
}
elseif( $_REQUEST['tipo'] == 'criterio')
{
    $titulo    = "Critérios";
    $subtitulo = "Selecione os Critérios";
}
if (array_key_exists('btnGravar', $_REQUEST ) ) 
{

 
    $perfis = arrayPerfil();
 
    if( ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis ) ) || ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis ) ) || ( in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis ) ) )
    {    	 
	    if( $_REQUEST['tipo'] == 'problema')
	    {
	        if( $_REQUEST['prodescricao'] != null )
	        {
	             
	            $prodescricao = $_REQUEST['prodescricao'];
	            $pdeid        = $_SESSION['pdeid'];
	            
	            $sql = "INSERT INTO 
	                        pdeescola.problema
	                            ( pdeid, prodescricao)
	                        VALUES
	                            ('$pdeid', '".$prodescricao."')";
	                            
	            $insere = $db->executar( $sql );

	            
		        $existe_preenchimento = $db->pegaUm("SELECT 
													pprid
												 FROM 
												 	pdeescola.pdepreenchimento
												 WHERE
												 	pdeid = '$pdeid'
													 AND
													 	ppritem = 'popupProblemaCriterio'");
				if( $existe_preenchimento == NULL )
				{
					$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento ,ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'si', 'popupProblemaCriterio','{$_SESSION['exercicio_atual']}')";
					$db->executar( $sql_p );

				}
			$db->commit();
			echo '<script type="text/javascript">alert("Operação Realizada com Sucesso!");window.opener.location.reload();window.close();</script>';  
	        exit();
	            
	        }
	        else 
	        {
	        	echo '<script type="text/javascript">alert("A descrição não pode estar em branco!");window.close();</script>';  
	        	exit();
	        }
	    }
	    elseif( $_REQUEST['tipo'] == 'criterio')
	    {
	          
	        for( $i = 0; $i < count( $_REQUEST['aceid'] ); $i++ )
	        {        
	            $aceid = $_REQUEST['aceid'][$i];
	            $proid = $_REQUEST['proid'];
	               
	            $sql = "INSERT INTO pdeescola.criterioeficacia ( proid, aceid ) VALUES( '$proid', '$aceid')";
	            $insert = $db->executar( $sql );
	            
	        }
	        $pdeid = $_SESSION["pdeid"];
	     	$existe_preenchimento = $db->pegaUm("SELECT 
													pprid
												 FROM 
												 	pdeescola.pdepreenchimento
												 WHERE
												 	pdeid = '$pdeid'
													 AND
													 	ppritem = 'popupProblemaCriterio'");
			if( $existe_preenchimento == NULL )
			{
				$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento ,ppritem) VALUES ( '$pdeid', 'si', 'popupProblemaCriterio')";
				$db->executar( $sql_p );
			}
		$db->commit();
		echo '<script type="text/javascript">alert("Operação Realizada com Sucesso!");window.opener.location.reload();window.close();</script>';  
	    exit();
	    }
    }
    else
    {
    	echo '<script type="text/javascript">alert("Você não tem permissão para inserir ou alterar dados nesta página");window.close();</script>';  
    	exit();
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

 
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
 </head>
  <body>

<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario" enctype="multipart/form-data">
    <table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: left"><?php echo $titulo; ?></div>
            <div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
              <table id="EtapasProducao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
                           
                <tr>
                  <td class="subtitulodireita">
                         <?php echo $subtitulo; ?>
                  </td>
                  
                    <?php
                    
                     if( $_REQUEST['tipo'] == 'problema')
                     {
                        ?>
                     <td align="center">
                         <?= campo_textarea( 'prodescricao', 'S', 'S', 'Descrição do Problema', 100 , 10, 500, $funcao = 'onKeyPress = "return controlar_foco( event );" onblur="retiraNewLine();"onkeyup=" retiraNewLine();"', $acao = 0, $txtdica = '', $tab = false ); ?>             
                     <?php
                     }
                     elseif( $_REQUEST['tipo'] == 'criterio')
                     {
                      
                      ?>
                      <td align="left" v-align="top">
                      <?php
                    $sql = "SELECT 
                                aceid as codigo, 
                                acedescricao as descricao 
                            FROM 
                                pdeescola.analisecriterioeficacia 
                            WHERE 
                                aceidpai IS NULL
                            and aceid not in (
                            select aceid from pdeescola.criterioeficacia WHERE proid = '{$_REQUEST['proid']}'
                            )
                            ";
                     $db->monta_checkbox('aceid[]', $sql, '', $separador='<br>'); 
                     }
                     
                     ?>
                     
                     </td>
                  </tr>
                  <tr>
                    <td colspan="2" align="center">
                        <input type="submit" name = "btnGravar" id = "btnGravar" value = "Salvar" /><input type="button" onClick="window.close();" name = "btnFechar" id = "btnFechar" value = "Fechar" />
                    </td>
                  </tr> 
                </div>
              </div>
            </td>
          </tr>
        </table>
     </form>
          
<script>
function controlar_foco( evento ) 
{ 
	if ( window.event || evento.which ) 
	{	
		if ( evento.keyCode == 13) 
		{ 	
			return false;
 
		}
	} 
	else 
	{
		return true;
	}
}

function retiraNewLine()
{
	var novaString = document.getElementById( "prodescricao" ).value;
                
	while( novaString.indexOf("\n") != -1 )
	{               
		novaString = novaString.replace('\n'," ");
    }        
    document.getElementById("prodescricao").value = novaString;
} 

</script>

    

