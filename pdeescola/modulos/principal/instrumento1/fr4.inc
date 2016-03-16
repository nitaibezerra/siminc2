<?php
pde_verificaSessao();
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");


function salva()
{
    global $db;
     
    /*
    echo("<pre>");
    print_r( $_REQUEST );
    echo("</pre>");
    die();
    */ 
    
    $disaltataxa = $_POST['disaltataxa'];
    
    $del = "UPDATE pdeescola.distorcaoidadeserie SET disaltataxa = 'f' WHERE pdeid = '{$_SESSION['pdeid']}'";
    $db->executar( $del );
     
    for( $i = 0; $i < count($disaltataxa); $i++ )
    {
        $arr = explode( '_',  $disaltataxa[$i] );
        
        $serie = $arr[1];
        $turno = $arr[2];
     
        $sql = "UPDATE
                pdeescola.distorcaoidadeserie
                SET
                    disaltataxa = 't'
                WHERE
                    sceid = '$serie'
                AND
                    titid = '$turno'
                AND
                    pdeid = '{$_SESSION['pdeid']}'";
        
        $db->executar( $sql );            
    }
    $pdeid = $_SESSION["pdeid"];
	$existe_preenchimento = $db->pegaUm("SELECT 
										pprid
									 FROM 
									 	pdeescola.pdepreenchimento
									 WHERE
									 	pdeid = '$pdeid'
										 AND
										 	ppritem = 'fr4'");
	if( $existe_preenchimento == NULL )
	{
		$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento ,ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'fr1', 'fr4','{$_SESSION['exercicio_atual']}')";
		$db->executar( $sql_p );
		 
	}
	$db->commit();  
}

$pdeid = ($_POST['pdeid']) ? $_POST['pdeid'] : $_SESSION['pdeid'];

if(!$pdeid) {
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include APPRAIZ.'includes/cabecalho.inc';
echo '<br />';
$titulo_modulo = 'Instrumento 1 - Ficha Resumo 1';
monta_titulo($titulo_modulo, '');

?>
<?=cabecalhoPDE();?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?=subCabecalho('4) Séries/Ciclos com altas taxas de distorção idade-série no Ensino Fundamental'); ?>
<form method="post" name="formulario" id="formFichaResumo">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<thead>
	<tr>
		<th width="30%">SÉRIE/ANO/CICLO</th>
		<th width="30%">TURNO</th>
		<th width="30%">TAXA DE DISTORÇÃO</th>
		<th width="10%">ALTA TAXA</th>
	</tr>
</thead>

<?php
#aqui virá o select
$dados = $db->carregar("SELECT 
                                        
                                            sceid,
                                            matfinal,
                                            sum(qtdalunos0) as qtd7anos,
                                            sum(qtdalunos1) as qtd8anos,
                                            sum(qtdalunos2) as qtd9anos,
                                            sum(qtdalunos3) as qtd10anos,
                                            sum(qtdalunos4) as qtd11anos,
                                            sum(qtdalunos5) as qtd12anos,
                                            sum(qtdalunos6) as qtdmais12anos,
                                            sum(qtdalunos7) as qtdate13anos,
                                            sum(qtdalunos8) as qtdate14anos,
                                            sum(qtdalunos9) as qtdate15anos,
                                            sum(qtdalunos10) as qtdate16anos,
                                            sum(qtdalunos11) as qtdmais16anos ,
                                            titid,
                                            disaltataxa
                                        FROM
                                        (
                                        
                                        
                                         SELECT 
                                         	sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            dis.disqtdalunos as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6,
                                            0 as qtdalunos7,
                                            0 as qtdalunos8,
                                            0 as qtdalunos9,
                                            0 as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            --dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 1 
                                            --AND titid = '{$turno[$y]['titid']}'
                                            
                                        
                                        UNION ALL
                                        
                                        
                                        SELECT 
                                            
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            dis.disqtdalunos as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6,
                                            0 as qtdalunos7,
                                            0 as qtdalunos8,
                                            0 as qtdalunos9,
                                            0 as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                           AND
                                           dis.faeid = 2  
                                            
                                        
                                      UNION ALL
                                        
                                        SELECT 
                                           
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            dis.disqtdalunos as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6,
                                            0 as qtdalunos7,
                                            0 as qtdalunos8,
                                            0 as qtdalunos9,
                                            0 as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND  
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                             AND
                                             dis.faeid = 3  
                                        
                                     UNION ALL
                                        
                                        SELECT
                                          
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            dis.disqtdalunos as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5, 
                                            0 as qtdalunos6,
                                            0 as qtdalunos7,
                                            0 as qtdalunos8,
                                            0 as qtdalunos9,
                                            0 as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND  
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                            AND
                                            dis.faeid = 4  
                                        
                                    UNION ALL
                                        
                                        SELECT 
                                         
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            dis.disqtdalunos as qtdalunos4,
                                            0 as qtdalunos5, 
                                            0 as qtdalunos6,
                                            0 as qtdalunos7,
                                            0 as qtdalunos8,
                                            0 as qtdalunos9,
                                            0 as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND  
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                            AND
                                            dis.faeid = 5  
                                        
                                   UNION ALL
                                        
                                        SELECT 
                                          
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
											0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            dis.disqtdalunos as qtdalunos5, 
                                            0 as qtdalunos6,
                                            0 as qtdalunos7,
                                            0 as qtdalunos8,
                                            0 as qtdalunos9,
                                            0 as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND  
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                             AND
                                             dis.faeid = 6  
                                        
                                    UNION ALL
                                        
                                        SELECT 
                                          
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            dis.disqtdalunos as qtdalunos6, 
                                            0 as qtdalunos7,
                                            0 as qtdalunos8,
                                            0 as qtdalunos9,
                                            0 as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND  
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                            AND
                                            dis.faeid = 7 
                                            
                                      UNION ALL
                                        
                                        SELECT 
                                       
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5, 
                                            0 as qtdalunos6,
                                            dis.disqtdalunos as qtdalunos7,
                                            0 as qtdalunos8,
                                            0 as qtdalunos9,
                                            0 as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND  
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                            AND
                                            dis.faeid = 8 
                                            
                                             UNION ALL
                                        
                                        SELECT 
                                         
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5, 
                                            0 as qtdalunos6, 
                                            0 as qtdalunos7,
                                            dis.disqtdalunos as qtdalunos8, 
                                            0 as qtdalunos9,
                                            0 as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND  
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                            AND
                                            dis.faeid = 9 
                                            
                                 UNION ALL
                                        
                                        SELECT 
                                         
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5, 
                                            0 as qtdalunos6, 
                                            0 as qtdalunos7, 
                                            0 as qtdalunos8,
                                            dis.disqtdalunos as qtdalunos9, 
                                            0 as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND  
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                            AND
                                            dis.faeid = 10 
                                            
                                   UNION ALL
                                        
                                        SELECT 
                                         
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5, 
                                            0 as qtdalunos6, 
                                            0 as qtdalunos7, 
                                            0 as qtdalunos8, 
                                            0 as qtdalunos9,
                                            dis.disqtdalunos as qtdalunos10,
                                            0 as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND  
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                            AND
                                            dis.faeid = 11 
                                            
                                     UNION ALL
                                        
                                        SELECT 
                                         
                                            sceid,
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5, 
                                            0 as qtdalunos6, 
                                            0 as qtdalunos7, 
                                            0 as qtdalunos8, 
                                            0 as qtdalunos9, 
                                            0 as qtdalunos10,
                                            dis.disqtdalunos as qtdalunos11,
                                            titid,
                                            disaltataxa
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND  
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." 
                                            AND
                                            dis.faeid = 12 
                                            
                                            ) as foo 
                                        GROUP BY  sceid, matfinal, titid, disaltataxa
                                        ORDER BY sceid, titid ");

                                            //condição para exibir msg qdo não existir dados.
	if($dados) {                                                                                                                             
      for($j=0; $j<count($dados); $j++) 
      {   
 
      				if( $dados[$j]['sceid'] == '1')
      				{
	                    $idadeSuperior = ($dados[$j]["qtd8anos"] + $dados[$j]["qtd9anos"] + $dados[$j]["qtd10anos"]                    
	                                      + $dados[$j]["qtd11anos"] + $dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"]);                       
      				}
            		elseif($dados[$j]['sceid'] == '2')
      				{
						$idadeSuperior = ($dados[$j]["qtd9anos"] + $dados[$j]["qtd10anos"]                    
	                                      + $dados[$j]["qtd11anos"] + $dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"]);    
      					
      				}
            		elseif($dados[$j]['sceid'] == '3')
      				{
						$idadeSuperior = ($dados[$j]["qtd10anos"]                    
	                                      + $dados[$j]["qtd11anos"] + $dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"]);    
      					
      				}
            		elseif($dados[$j]['sceid'] == '4')
      				{
						$idadeSuperior = ($dados[$j]["qtd11anos"] + $dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"] );    
      					
      				}
      				elseif($dados[$j]['sceid'] == '5')
      				{
						$idadeSuperior = ($dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"]);    
      					
      				}
      				
      
      				elseif($dados[$j]['sceid'] == '6')
      				{
						$idadeSuperior = ($dados[$j]["qtdate13anos"] + $dados[$j]["qtdate14anos"] + $dados[$j]["qtdate15anos"]       
	                                      + $dados[$j]["qtdate16anos"] + $dados[$j]["qtdmais16anos"]);      
      					
      				}
      				elseif($dados[$j]['sceid'] == '7')
      				{
						$idadeSuperior = ($dados[$j]["qtdate14anos"] + $dados[$j]["qtdate15anos"]       
	                                      + $dados[$j]["qtdate16anos"] + $dados[$j]["qtdmais16anos"]);      
      				}
      				elseif($dados[$j]['sceid'] == '8')
      				{
						$idadeSuperior = ($dados[$j]["qtdate15anos"] 
	                                      + $dados[$j]["qtdate16anos"] + $dados[$j]["qtdmais16anos"]);      
      				}
      				elseif($dados[$j]['sceid'] == '9')
      				{
						$idadeSuperior = ($dados[$j]["qtdate16anos"] + $dados[$j]["qtdmais16anos"]);    
      					
      				}

      				 
      				
      				
                    $idadeSuperior = ($idadeSuperior == 0) ? '' : $idadeSuperior;                                                  
                                                                                                                                             
                                                                                                                                             
                    if(($dados[$j]["matfinal"] != '') AND ($dados[$j]["matfinal"] != 0))
                    {
                        $taxa = str_replace('.',',',number_format((($idadeSuperior / $dados[$j]["matfinal"]) * 100),2))."%";
                    	$taxaDistorcao = round( $taxa );
                    }
                    else
                    {
                        $taxaDistorcao = "";
                    }
                                           
                    $totalMatFinal         += $dados[$j]["matfinal"];
                    $total8anos            += $dados[$j]["qtd8anos"];
                    $total9anos            += $dados[$j]["qtd9anos"];
                    $total10anos           += $dados[$j]["qtd10anos"];
                    $total11anos           += $dados[$j]["qtd11anos"];
                    $total12anos           += $dados[$j]["qtd12anos"];
                    $totalMais12anos       += $dados[$j]["qtdmais12anos"];
                    $totalate13anos        += $dados[$j]["qtdate13anos"];
                    $totalate14anos        += $dados[$j]["qtdate14anos"];
                    $totalate15anos        += $dados[$j]["qtdate15anos"];
                    $totalate16anos        += $dados[$j]["qtdate16anos"];
                    $totalMais16anos       += $dados[$j]["qtdmais16anos"]; 
                    $totalIdadeSuperior    += $idadeSuperior;
                    
                    
			if(($dados[$j]["matfinal"] != '') && ( $dados[$j]["matfinal"] != 0))
			{
                $totalTaxaDistorcao = str_replace(',','.',number_format((($idadeSuperior / $dados[$j]["matfinal"]) * 100)));
            	//$totalTaxaDistorcao = round( $totalTaxa );
			}
            else
            {
                $totalTaxaDistorcao = 0;
            }
            ?>
            <tr>

                <td>
                    <?php echo $dados[$j]['sceid']."°"; ?>
                </td>             
                <td>
                   <?php 
                   switch( $dados[$j]['titid'] )
                   {
                   case 1: 
                        $turno = "Matutino";
                        break;
                   case 2: 
                        $turno = "Vespertino";
                        break;
                   case 3: 
                        $turno = "Noturno";
                        break;
                   case 4: 
                        $turno = "Integral";
                        break;
                   }
                   echo $turno;
                   ?>
                </td>              
                <td>
                   <?php echo $totalTaxaDistorcao.'%'; ?>
                </td>
                <td>
                 <center>
	                  <?php
		                /*
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
		                            
		                       -- AND char_length(acecodigo) < 4
		                        ";
		                */
               	 	  ?>
                
                <input type ="checkbox" name="disaltataxa[]" <?php  if ( $dados[$j]['disaltataxa'] == 't' )  echo("checked = 'checked'"); ?> value="disaltataxa_<?php echo $dados[$j]['sceid']; ?>_<?php echo $dados[$j]['titid']; ?>" id="disaltataxa">
             
                <?php
             	// $db->monta_checkbox('disaltataxa[]', $sql, '', $separador='<br>'); 
                ?>
              </center>
             </td>
            </tr>
            
            <?php
		 }
	}else {
		echo "<tr align=\"center\">
			  	<td bgcolor=\"#f4f4f4\" colspan=\"6\">
			  		<br />
			  		<div style=\"color:red;\">Não existem registros cadastrados.</div>
			  		<br />
			  	</td>
			  </tr>";
		}
      ?>
                 
<tbody>
	
</tbody>
<tfoot>
<? echo $cForm->montarbuttons('validaForm()'); ?>
</tfoot>
</table>
</form>
<script>
function validaForm()
{
    
    document.formulario.submit();
    return true;
    
}
</script>
    