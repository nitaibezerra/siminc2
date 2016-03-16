<?php
# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();

#popupProblemaCriterio

if( $_REQUEST['tipo'] == 'objetivo' )
{
    $titulo    = "Cadastro de Objetivos";
    $subtitulo = "Descrição do Objetivo";
}
elseif( $_REQUEST['tipo'] == 'estrategia')
{
    $titulo    = "Cadastro de Estratégias";
    $subtitulo = "Descrição da Estratégia";
}
elseif( $_REQUEST['tipo'] == 'meta')
{
    $titulo    = "Cadastro de Metas";
    $subtitulo = "Descrição da Meta";
}
elseif( $_REQUEST['edit'] )
{
    $titulo    = "Alterar";
    $subtitulo = "Nova descrição";
}

$pseidpai     = $_REQUEST['pseidpai'];
$pdeid        = $_SESSION['pdeid'];
$pdeidGet	  = $_GET['pdeid'];

if ($pdeid != $pdeidGet){
	die('<script>
			alert(\'Sistema não permite acessar duas escolas simultaneamente!\nTente novamente.\');
			window.opener.location = "pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A";
			window.close();
		 </script>');
}

if (array_key_exists('btnGravar', $_REQUEST ) ) 
{
    $perfis = arrayPerfil();
 
    if( ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis ) ) || ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis ) ) || ( in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis ) ) )
    { 
	    if( $_REQUEST['psedescricao'] != null )
	    {
	         
	        $psedescricao = $_REQUEST['psedescricao'];
	        $pdeid        = $_SESSION['pdeid'];
	        
	        $arrDatai = explode( "/", $_REQUEST['pladatainicio']);
	        $mesI = $arrDatai[0];
	        $anoI = $arrDatai[1];
	        
	        $dataI = $anoI.'-'.$mesI.'-01';
	        
	        $arrDataF = explode( "/", $_REQUEST['pladatatermino']);
	        $mesF = $arrDataF[0];
	        $anoF = $arrDataF[1];
	        
	        $dataF = $anoF.'-'.$mesF.'-01';

	       if( $_REQUEST['edit'] )
	       {
	       		if( $_REQUEST['tipo'] != 'meta')
	       		{
	                $sql = "UPDATE
	                		    pdeescola.planosuporteestrategico
	                        SET
	                            psedescricao = '".substr($psedescricao, 0, 500 )."'
	                        WHERE
	                            pseid = '{$_REQUEST['edit']}'
	                        ";
	                            	
	                $db->executar( $sql );
	       		}
	       		else
	       		{
	       			$sql = "UPDATE 
	       						pdeescola.planosuporteestrategico
	                        SET
	                            psedescricao = '".substr($psedescricao, 0, 500 )."'
	                        WHERE
	                            pseid = '{$_REQUEST['edit']}'
	                        ";
	                               
	                $db->executar( $sql );
	                
	                $sqlEditPlano = "UPDATE pdeescola.planoacao
	                				 SET 
	                				 plaliderobjetivo 	 = '".substr($_REQUEST['plaliderobjetivo'], 0, 100 )."',
		                			 plaindicadormeta 	 = '".substr($_REQUEST['plaindicadormeta'], 0, 500 )."',
		                			 plagerenteplanoacao = '".substr($_REQUEST['plagerenteplanoacao'], 0, 100 )."',
		                			 pladatainicio 		 = '".str_replace( ' ', '',$dataI)."',
		                			 tirid		 		 = '{$_REQUEST['tirid']}',
		                			 pladatatermino 	 = '".str_replace( ' ', '',$dataF)."'
		                			 WHERE
		                			 pseid = '{$_REQUEST['edit']}'";
		                			 
			       $db->executar( $sqlEditPlano );
	       		}							
	        }
	        else
	        {
	            // inserir objetivo
	            if ( $pseidpai == '' )
	            {
	            
	                $sqlVerifica = "SELECT * FROM pdeescola.planosuporteestrategico  WHERE pdeid = '$pdeid' AND pseidpai  IS NULL ";
	                $esgotado = $db->carregar( $sqlVerifica );
	            	
	                if( count( $esgotado ) < 3)
	                {
	                    $sql = "INSERT INTO pdeescola.planosuporteestrategico
	                            (pdeid,  
	                            psedescricao
	                            )
	                        	VALUES
	                            (
	                            '$pdeid', 
	                            '".substr($psedescricao, 0, 500 )."'
	                            )";

	                            
	                     $insere = $db->executar( $sql );
	                }
	                else
	                {                
	                     $negado = 1;
	                     $db->rollback();
	                     echo("<script>alert('Não é possivel cadastrar mais de 3 Objetivos para esta escola');window.close();</script>");
	                     exit();
	                }
	            } 
	            else
	            {
	            	//inserir estrategia
	                if( $_REQUEST['tipo'] != 'meta')
		            {   
		             $sql = "SELECT pseid FROM pdeescola.planosuporteestrategico WHERE pseid = ". $pseidpai;
		             $pseidPaiExiste = $db->pegaUm($sql);
		           
		               if($pseidPaiExiste){    
		                $sql = "INSERT INTO pdeescola.planosuporteestrategico
		                        (pdeid, 
		                        pseidpai, 
		                        psedescricao
		                        )
		                        VALUES
		                        (
		                        '$pdeid', 
		                        '$pseidpai', 
		                        '".substr($psedescricao, 0, 500 )."'
		                        )";
		                        
		                $insere = $db->executar( $sql );
		               }else{
		            		echo '<script type="text/javascript">alert("Esse Objetivo não existe mais.. Tente novamente!");</script>';
					    	echo '<script type="text/javascript">window.opener.location.reload(); window.close();</script>';
					    	exit(); 
		               		}  
		            }
		            else
		            {	
		            	$sql = "SELECT pseid FROM pdeescola.planosuporteestrategico WHERE pseid = ". $pseidpai;
		            	$pseidPaiExiste = $db->pegaUm($sql);

		                //inserir meta
		              if($pseidPaiExiste){
		            	$sql = "INSERT INTO pdeescola.planosuporteestrategico
		                        (pdeid, 
		                        pseidpai, 
		                        psedescricao
		                        )
		                        VALUES
		                        (
		                        '$pdeid', 
		                        '$pseidpai', 
		                        '".substr($psedescricao, 0, 500 )."'
		                        )
		                        RETURNING pseid";
		                      
		                $pseid = $db->pegaUm( $sql );
		              }else{
		            		echo '<script type="text/javascript">alert("Essa estratégia não existe mais.. Tente novamente!");</script>';
					    	echo '<script type="text/javascript">window.opener.location.reload(); window.close();</script>';
					    	exit(); 
		                   }
		                $sqlplano = "INSERT INTO pdeescola.planoacao
		                			(pseid,
		                			 plaliderobjetivo,
		                			 plaindicadormeta,
		                			 plagerenteplanoacao,
		                			 pladatainicio,
		                			 tirid,
		                			 pladatatermino
		                			 )
		                			 VALUES
		                			 (
		                			  '$pseid',
		                			  '".substr($_REQUEST['plaliderobjetivo'], 0, 100 )."',
		                			  '".substr($_REQUEST['plaindicadormeta'], 0, 500 )."',
		                			  '".substr($_REQUEST['plagerenteplanoacao'], 0, 100 )."',
		                			  '".str_replace( ' ', '',$dataI)."',
		                			  '{$_REQUEST['tirid']}',
		                			  '".str_replace( ' ', '',$dataF)."'
		                			 )
		                			 ";
		                
		            	$insertPlano = $db->executar( $sqlplano );
		            	
		            	//inserindo na tabela de verificação de preenchimento do pde
		            	$existe_preenchimento = $db->pegaUm("SELECT 
																	pprid
															 FROM 
										 							pdeescola.pdepreenchimento
															 WHERE
										 							pdeid = '$pdeid'
											 				 AND
											 						ppritem = 'popupObjetivosMetas'");
						if( $existe_preenchimento == NULL )
						{
							$sql_p = "INSERT INTO pdeescola.pdepreenchimento
									  ( pdeid, 
									  pprinstrumento,
									  ppritem, 
									  pdeanoreferencia) 
									  VALUES 
									  ( '$pdeid', 
									  'pa', 
									  'popupObjetivosMetas',
									  '{$_SESSION['exercicio_atual']}'
									  )
									  ";
							$db->executar( $sql_p );
						}       
		            	
		            }
		        }  
	            
	        }
	    $db->commit();
	    echo '<script type="text/javascript">alert("Operação Realizada com Sucesso!");</script>';
	    echo '<script type="text/javascript">window.opener.location.reload(); window.close();</script>';
	    exit();  
	}
	else
    {
    	echo '<script type="text/javascript">alert("A descrição não pode estar em branco!");window.opener.location.reload(); window.close();</script>';
    	exit();  
    }
  }
  echo '<script type="text/javascript">alert("Você não tem permissão para inserir ou alterar dados nesta página");window.opener.location.reload(); window.close();</script>';
  exit();  
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
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
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
                    
           
                         // $prodescricao = $rsPro[0]["prodescricao"];
                     ?>
                     <td align="left">  
                     <?php
                     
                     
                      
                     
                     if( $_REQUEST['edit'] )

                     {
                         $sql = "SELECT 
                         			psedescricao 
                         		 FROM 
                         		 	pdeescola.planosuporteestrategico 
                         		 WHERE 
                         		 	pseid = '{$_REQUEST['edit']}'";
                         		 		
                         $psedescricao = $db->pegaUm( $sql );
                         
	                     	 if( $_REQUEST['tipo'] == 'meta')
	                         {
	                         $sqlplano = "SELECT 
												plaid, 
												pseid,
												plaliderobjetivo,
												plaindicadormeta,
												plagerenteplanoacao,
												to_char(pladatainicio::date,'MM/YYYY') AS pladatainicio,
												tirid,
												to_char(pladatatermino::date,'MM/YYYY') AS pladatatermino
	                         			 FROM 
	                         			 		pdeescola.planoacao 
	                         			 WHERE 
	                         			 		pseid = '{$_REQUEST['edit']}'
	                         			 ";
	                               			 $ra = $db->carregar( $sqlplano );

	                        
	                         }
						 
                     }
                     ?>

                         <?= campo_textarea( 'psedescricao', 'S', 'S', 'Descrição ', 100 , 10, 500, 'id="psedescricao" onKeyPress = "return controlar_foco( event );" onblur="retiraNewLine();"onkeyup=" retiraNewLine();" ', $acao = 0, $txtdica = '', $tab = false ); ?>             

						<!-- 
						 <?= campo_texto('psedescricao', 'S', $somenteLeitura, '', 100, 400, '', '', 'left', '',  0, 'id="psedescricao" onblur="MouseBlur(this);"' ); ?>
						-->         		                
                     </td>
                  </tr>
                  <?php
					if( $_REQUEST['tipo'] == 'meta')
					{
					?>
						
					<tr>
						<td class="subtitulodireita">
							Líder do Objetivo:
						</td> 
						<td align="left">
							<?php
							$plaliderobjetivo = $ra[0]['plaliderobjetivo'];
							?>
							<?= campo_texto('plaliderobjetivo', 'S', $somenteLeitura, '', 97, 100, '', '', 'left', '',  0, 'id="plaliderobjetivo" onblur="MouseBlur(this);"' ); ?>
                		</td>
                	</tr>	
                	<tr>
		                  <td class="subtitulodireita">
		                        Indicador da Meta:
		                  </td>
	                  
	                     <td align="left">  
	                     	<?php
							$plaindicadormeta = $ra[0]['plaindicadormeta'];
							?>
	                        <?= campo_textarea( 'plaindicadormeta', 'S', 'S', 'Descrição ', 100 , 10, 500,'id="plaindicadormeta"', $funcao = '', $acao = 0, $txtdica = '', $tab = false ); ?>             
	                     </td>
                  	</tr>
                  	<tr>
						<td class="subtitulodireita">
							Gerente do PLano de Ação:
						</td> 
						<td align="left">
							<?php
							$plagerenteplanoacao = $ra[0]['plagerenteplanoacao'];
							?>
							<?= campo_texto('plagerenteplanoacao', 'N', $somenteLeitura, '', 97, 100, '', '', 'left', '',  0, 'id="plagerenteplanoacao" onblur="MouseBlur(this);"' ); ?>
                		</td>
                	</tr>	
                	
                	<tr>
						<td class="subtitulodireita">
							Início:
						</td> 
						<td align="left">
							<?php
							$pladatainicio = $ra[0]['pladatainicio'];
							?>
							
                			<?= campo_texto('pladatainicio', 'S', $somenteLeitura, '', 30, 100, '##/####', '', 'left', '',  0, 'id="pladatainicio" onblur="MouseBlur(this);"', '', '', 'validarMesAno( this )' ); ?>
                		
                		</td>
                	</tr>	
                	
                	<tr>
						<td class="SubTituloDireita">
							Tipo de Revisão
						</td>
						<td align="left">
						<?php
						$sql = "SELECT 
									tirid as codigo,
									tirdescricao as descricao
								FROM 
									pdeescola.tiporevisao";
						
						$tirid = $ra[0]['tirid'];
						
			        	$db->monta_combo("tirid",$sql,'S',"Selecione o Tipo de Revisão",'','', '', 200, '', 'tirid');
						?>	
						</td>
					</tr>
                	
                	<tr>
						<td class="subtitulodireita">
							Término:
						</td> 
						<td align="left">
							<?php
							$pladatatermino = $ra[0]['pladatatermino'];
							?>
							<?= campo_texto('pladatatermino', 'S', $somenteLeitura, '', 30, 100, '##/####', '', 'left', '',  0, 'id="pladatatermino" onblur="MouseBlur(this);"', '', '', 'validarMesAno( this )' ); ?>
                		
						</td>
                	</tr>	
							
													
					<?php
					} 
                
                  ?>
                  <tr>
                    <td colspan="2" align="center">
                        <?php
                    	if( $_REQUEST['tipo'] == 'meta' )
                    	{
                    	?>
                    		<input type="submit"  onclick="return validarForm();" name = "btnGravar" id = "btnGravar" value = "Salvar" /><input type="button" onClick="window.close();" name = "btnFechar" id = "btnFechar" value = "Fechar" />	
                    	<?
						}
						else
						{
                        ?>
                        	<input type="submit" name = "btnGravar" id = "btnGravar" value = "Salvar" onclick="return validarForm2();" /><input type="button" onClick="window.close();" name = "btnFechar" id = "btnFechar" value = "Fechar" />	
                    	<?php
						}
                    	?>
                    </td>
                  </tr>
               </table>
             </div>
           </td>
         </tr>
      </table>
 </form>
     
<script>

function valida_Data()
{
	var datai = formulario.pladatainicio.value;
	var datainum = formulario.pladatainicio.value.length;
    var dataf = formulario.pladatatermino.value;
  	var datafnum = formulario.pladatatermino.value.length;
  	var arrdatai = datai.split( "/" );
   
    if( Number(datafnum) < 7 ){
    	alert('É necessário preencher uma data no formato: mm/aaaa');
    	return false;
    }
    
    if( Number(datainum) < 7 ){
    	alert('É necessário preencher uma data no formato: mm/aaaa');
    	return false;
    }
    
    if( datainum != 7 )
  	{
  		alert('Digite uma data no formato: mm/aaaa');
  		return false;
  	}
  	
  	if( datafnum != 7 )
  	{
  		alert('Digite uma data no formato: mm/aaaa');
  		return false;
  	}

    var mesi = parseFloat(arrdatai[0]);
    var anoi = parseFloat(arrdatai[1]);
  
    if(( mesi > 12 ) || (mesi == 0))
    {
        alert('Data inexistente para campo Início');
        return false;
    }
    
    if(anoi == 0)
    {
        alert('Data inexistente para campo Início');
        return false;
    }
    
    if(anoi < 1900) 
    {
        alert('Ano não permitido para Data de Início.');
        return false;
    }

    var arrdataf = dataf.split( "/" );
    var mesf = parseFloat(arrdataf[0]);
    var anof = parseFloat(arrdataf[1]);
    
    if(( mesf > 12 ) || (mesf == 0))
    {
        alert('Data inexistente para campo Término');
        return false;
    }

    if(anof == 0)
    {
        alert('Data inexistente para campo Término');
        return false;
    }
    
    if(anof < 1900) 
    {
        alert('Ano não permitido para Data de Término.');
        return false;
    }
    
    if(anoi > anof) 
    {
        alert('A data inicial não pode ser maior do que a data final.');
        return false;
    }
    else if (anoi == anof) 
    {
        if(mesi > mesf) 
        {
            alert('A data inicial não pode ser maior do que a data final.');
            return false;
        }
    }
    return true;
}

function validarForm2()
{
	var psed = document.formulario.psedescricao.value.length ;
		if( psed >= 500 ){
			alert('Texto muito grande. Limite máximo de 500 caracteres.');
			return false;
		}else{
			return true;
		}
	var psedescricao = document.getElementById('psedescricao').value;
	     if( psedescricao == '' )
	     {
		     alert('É necessário informar Descrição da Meta');
		     return false;
	     }
}

function validarForm()
{
	retiraNewLine();
	
	var psed = document.formulario.psedescricao.value.length ;
	var plaim = document.formulario.plaindicadormeta.value.length ;
	
	if((psed >= 500) || (plaim >= 500)){
		alert('Texto muito grande. Limite máximo de 500 caracteres.');
		return false;
	}
	
	 if( valida_Data() )
	 {
	 	 var psedescricao = document.getElementById('psedescricao').value;
	     if( psedescricao == '' )
	     {
		     alert('É necessário informar Descrição da Meta');
		     return false;
	     }
	 
		 var tirid = document.getElementById('tirid').value;
	     if( tirid == '' )
	     {
		     alert('É necessário informar um tipo de revisão');
		     return false;
	     }
	     	
	     var plaliderobjetivo = document.getElementById('plaliderobjetivo').value;
		 if( plaliderobjetivo == '' )
		 {
		     alert('É necessário informar um líder de Objetivo');
		     return false;
		 }
	     var plaindicadormeta = document.getElementById('plaindicadormeta').value;
	     if( plaindicadormeta == '' )
	     {
		     alert('É necessário informar o indicador da meta');
		     return false;
	     }
	     
	     var pladatainicio = document.getElementById('pladatainicio').value;
	     if( pladatainicio == '' )
	     {
		     alert('É necessário informar uma data de início');
		     return false;
	     }

	     var pladatatermino = document.getElementById('pladatatermino').value;
	     if( pladatatermino == '' )
	     {
		     alert('É necessário informar uma data de termino');
		     return false;
	     }
	         
	        //tratando também dentro do valida_Data
   			if( !validarMesAno( document.getElementById( 'dpadatainicio'  ) ) ) return false;
   			if( !validarMesAno( document.getElementById( 'dpadatatermino' ) ) ) return false;
    
   			return true;

     }
     else
     {
     	return false;
     }   
          
}

//function verificaEnter(key)
//{
//	if( key == '13')
//	{
//		alert('Não é permitido o uso de paragrafos para a inserção de elementos na árvore');
//		
//		 
//	    document.getElementById('psedescricao').value = '';
//		
//		return false;
//	}	 
//}


function controlar_foco( evento ) 
{ 
//alert(evento.keyCode);
	if ( window.event || evento.which ) 
	{	
		if ( evento.keyCode == 13) 
		{ 	
			return false;
			//return document.getElementById('btnFechar').focus();
			//return document.formulario.reset();
		}
	} 
	else 
	{
		return true;
	}
}

function retiraNewLine()
{ 
	var novaString = document.getElementById( "psedescricao" ).value;
        
	while( novaString.indexOf("\n") != -1 )
	{               
		novaString = novaString.replace('\n', " ");
    }        
	while( novaString.indexOf("\r") != -1 )
	{               
		novaString = novaString.replace('\r', "");
    }        

    document.getElementById("psedescricao").value = novaString;
    
} 

//verificando data no onblur
function validarMesAno( obj ){

	obj.value = mascaraglobal( '##/####', obj.value ); 

	if( obj.value ) {
	
		var valorInicial = obj.value;
		
		obj.value = '01/'+obj.value;
		
		if( !validaData( obj ) ){
			obj.value = "";
			alert( "Data Inválida" );
			return false;
		}
		obj.value = valorInicial;
		return true;
	}
	
}

//
//function somenteNumeros(e) {	
//	if(window.event) {
//    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
//        key = e.keyCode;
//    }
//    else if(e.which) {
//    	/* Netscape */
//        key = e.which;
//    }
//    if(key!=8 || key < 48 || key > 57) return (((key > 47) && (key < 58)) || (key==8) || (key==9));
//    {
//    	return true;
//    }
//} 


</script>