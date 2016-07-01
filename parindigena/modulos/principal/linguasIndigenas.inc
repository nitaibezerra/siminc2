<?php
// arquivo "povosIndigenas.inc"
include APPRAIZ . 'includes/Agrupador.php';
//$db->executar("SET CLIENT_ENCODING TO 'ISO-8859-1'");

//aqui dentro o código para inserçao, alteraçao e exclusao


if ($_REQUEST['alterar']) {
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT
                *
            FROM 
                parindigena.linguaindigena 
            WHERE
                linid = {$_REQUEST['alterar']}";               
    
    $rsDadosLingua = $db->carregar( $sql );
}

if ($_REQUEST['excluir'])
{    
       //verificando a integridade para poder excluir o povo indigena   
      
        $sql = "SELECT 
                    l.linid
                FROM 
                    parindigena.linguaindigena AS l
                LEFT JOIN 
                    parindigena.linguaformacaoinicial AS lfi 
                        ON l.linid = lfi.linid
                LEFT JOIN 
                    parindigena.linguaformacaocontinuadaetapa AS lfc 
                        ON l.linid = lfc.linid
                LEFT JOIN 
                    parindigena.linguamaterialdidatico AS lm 
                        ON l.linid = lm.linid
                WHERE
                    l.linid = {$_REQUEST['excluir']}";
              
         $numRegistros = $db->carregar($sql);       
                  
         $num = count($numRegistros);
         
         //dbg($num);
         //die();
         //se o count do array do resultado for maior que 0, entao nao sera possivel a exclusao!.
         if ($num > 1 ) 
         {                         
             echo("<script>alert('Nao é possivel a exclusao desta Lingua Indígena, pois a mesma possui um ou mais vínculos');\n</script>");
             $var = 1;   
         }
         else
         { 
         
             $sql = "DELETE FROM parindigena.linguaindigena WHERE linid = {$_GET['excluir']}";
            
             $db->executar( $sql );
             $db->commit();
             
             header('Location: parindigena.php?modulo=principal/linguasIndigenas&acao=I');
          }
          if ($var == 1)
          {              
              echo '<script type="text/javascript">window.location.href = "parindigena.php?modulo=principal/linguasIndigenas&acao=I";</script>';
          }
           
     }

 if (array_key_exists('btnGravar', $_REQUEST))
{
    if($_REQUEST['linnome']!='')
    {    //faz o insert ou update
        
         if($_REQUEST['alterar'])
         {
                 $sql = "UPDATE 
                            parindigena.linguaindigena 
                         SET 
                            linnome = '{$_REQUEST['linnome']}' 
                         WHERE 
                            linid = '{$_REQUEST['alterar']}'";
                 
                            $db->carregar($sql);
                            $db->commit();
                            
                            header('Location: parindigena.php?modulo=principal/linguasIndigenas&acao=I');
         }
        $linnome = strtoupper($_REQUEST['linnome']); 
        //verifica se ja existe uma unidade com a mesma descriçao e tipo
        $sql = "SELECT * FROM 
                    parindigena.linguaindigena 
                WHERE 
                    UPPER(linnome) = '$linnome' 
                ";
                    
        $arrReg = $db->carregar($sql);
        

        $numReg = count($arrReg[0]);
        
        
        //se o numero de registros encontrados for maior do que zero
        if( $arrReg && $numReg > 0 )
        {
            echo("<script type=\"text/javascript\">alert('Esta Lingua Indígena já está cadastrada!');</script>");
        }
        else
        {                 
         
            $sql = "INSERT INTO 
                          parindigena.linguaindigena (linnome) 
                    VALUES ( 
                              '{$_REQUEST['linnome']}' 
                            )";
                            
                                                                     
            $db->executar($sql);
            $db->commit();
            
             alert('Dados gravados com sucesso!');
            
        }
             //dbg($sql);
             //die();
             

           // header('Location: parindigena.php?modulo=principal/linguasIndigenas&acao=I');

        }
   }

     


include APPRAIZ . 'includes/cabecalho.inc';
?>
<br>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Línguas Indígenas</label></td></tr><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td></tr></table>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadLinguaIndigena">
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         
            <tr>
                <td class ="SubTituloDireita" align"right"> Língua Indígena: </td>
                <td> 
                     <?php
                    $linnome = $rsDadosLingua[0]["linnome"];
                     ?>
                     <?= campo_texto('linnome', 'N', $somenteLeitura, '', 100, 600, '', '', 'left', '',  0, 'id="linnome" onblur="MouseBlur(this);"' ); ?>
                </td>
                <td align="left">
                     <input type="submit" name="btnBuscar" value="Buscar" />
                     <input type="submit" name="btnGravar" value="Salvar" />
                </td>
            </tr>
          
          </form>
          <tr>
            <td class="SubTituloDireita" colspan = "3">
                <?php
                
                if (array_key_exists('btnBuscar', $_REQUEST))
                {
                    $sql = "SELECT
                                    '<img onclick=\"return alterarLinguaIndigena('|| linid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirLinguaIndigena('|| linid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    linid, 
                                    linnome
                                FROM 
                                     parindigena.linguaindigena";
                                     
                     if( strtoupper($_REQUEST['linnome']))
                     {     
                     $sql.=" WHERE 
                                    UPPER(linnome)  ILIKE '%" .strtoupper($_REQUEST['linnome']). "%' ";
                     }
                   
                     if( strtoupper($_REQUEST['linnome']))
                     {     
                     $sql.=" ORDER BY
                                    linnome ";
                     }               
                                    
                     $buscar = 1;
                }
                else
                {
                    $sql = "SELECT
                                    '<img onclick=\"return alterarLinguaIndigena('|| linid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirLinguaIndigena('|| linid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    linid,
                                    linnome
                                FROM 
                                     parindigena.linguaindigena";  
                }
                $cabecalho = array( "Opções", "Código ", " Língua Indígena" );
                
                
                
        
                $db->monta_lista($sql, $cabecalho, 20, 10, 'N', '', '' );
                
                ?>
            </td>
           </tr>
        
       </table>
       
 <script type="text/javascript">

/* Funçao para validar os dados do formulario requisitante.
 * author: PedroDantas
 * date: 2008-08-25
 */
function validaForm()
{  
    var total = cadLinguaIndigena.elements.length;
    i = 0;
    while (i <= total)
     {
         if(cadLinguaIndigena.elements[i].value == "")
         {
           alert("É necessário o preenchimento de todos os campos");
           
           cadLinguaIndigena.elements[i].focus();
           return false;
           break;
         }
        i++;
     }   
 }
 
 /* Funçao para excluir uma unidade de medida
 * author: PedroDantas
 * date: 2008-08-25
 * params: undid (id da unidade selecionada)
 */
 function excluirLinguaIndigena(linid)
         {
             if (confirm('Deseja excluir o registro?')) 
             {
                 return window.location.href = 'parindigena.php?modulo=principal/linguasIndigenas&acao=I&excluir=' + linid;
                
             }
             
         }
         
function alterarLinguaIndigena(linid)
{    
    return window.location.href = 'parindigena.php?modulo=principal/linguasIndigenas&acao=I&alterar=' + linid;
}
 
 </script>
