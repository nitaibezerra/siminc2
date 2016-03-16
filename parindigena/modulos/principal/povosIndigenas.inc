<?php
// arquivo "povosIndigenas.inc"
include APPRAIZ . 'includes/Agrupador.php';
//$db->executar("SET CLIENT_ENCODING TO 'ISO-8859-1'");

//aqui dentro o código para inserçao, alteraçao e exclusao


if ($_REQUEST['alterar']) 
{
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT
                *
            FROM 
                parindigena.povoindigena 
            WHERE
                povid = {$_REQUEST['alterar']}";               
    
    $rsDadosPovo = $db->carregar( $sql );
}

if ($_REQUEST['excluir'])
{    
       //verificando a integridade para poder excluir o povo indigena   
          
        $sql = "SELECT 
                    p.povid
                FROM 
                    parindigena.povoindigena AS p
                LEFT JOIN 
                    parindigena.povoformacaoinicial AS pfi 
                        ON p.povid = pfi.povid
                LEFT JOIN 
                    parindigena.povoformacaocontinuadaetapa AS pfc 
                        ON p.povid = pfc.povid
                LEFT JOIN 
                    parindigena.povomaterialdidatico AS pm 
                        ON p.povid = pm.povid
                WHERE
                    p.povid = {$_REQUEST['excluir']}";
              
         $numRegistros = $db->carregar($sql);       
                  
         $num = count($numRegistros);
         
         //dbg($num);
         //die();
         //se o count do array do resultado for maior que 0, entao nao sera possivel a exclusao!.
         if ($num > 1 ) 
         {                         
             echo("<script>alert('Nao é possivel a exclusao deste Povo Indígena, pois o mesmo possui um ou mais vínculos');\n</script>");
             $var = 1;   
         }
         else
         { 
          
             $sql = "DELETE FROM parindigena.povoindigena WHERE povid = {$_GET['excluir']}";
            
             $db->executar( $sql );
             $db->commit();
             
             header('Location: parindigena.php?modulo=principal/povosIndigenas&acao=I');
         }  
         if ($var == 1)
         {              
             echo '<script type="text/javascript">window.location.href = "parindigena.php?modulo=principal/povosIndigenas&acao=I";</script>';
         }
         
           
}

 if (array_key_exists('btnGravar', $_REQUEST))
{
    if($_REQUEST['povnome']!='')
    {    //faz o insert ou update
        
         if($_REQUEST['alterar'])
         {
                 $sql = "UPDATE 
                            parindigena.povoindigena 
                         SET 
                            povnome = '{$_REQUEST['povnome']}' 
                         WHERE 
                            povid = '{$_REQUEST['alterar']}'";
                 
                            $db->carregar($sql);
                            $db->commit();
                            
                            header('Location: parindigena.php?modulo=principal/PovosIndigenas&acao=I');
        }
        $povnome = strtoupper($_REQUEST['povnome']); 
        //verifica se ja existe uma unidade com a mesma descriçao e tipo
        $sql = "SELECT * FROM 
                    parindigena.povoindigena 
                WHERE 
                    UPPER(povnome) = '$povnome' 
                ";
                    
        $arrReg = $db->carregar($sql);
        

        $numReg = count($arrReg[0]);
        
        
        //se o numero de registros encontrados for maior do que zero
        if( $arrReg && $numReg > 0 )
        {
            echo("<script type=\"text/javascript\">alert('Este Povo Indígena já está cadastrado!');</script>");
        }
        else
        {                 
         
            $sql = "INSERT INTO 
                          parindigena.povoindigena (povnome) 
                    VALUES ( 
                              '{$_REQUEST['povnome']}' 
                            )";
            $db->executar($sql);
            $db->commit();
            
             alert('Dados gravados com sucesso!');
            
        }
             //dbg($sql);
             //die();
                                         

          //  header('Location: parindigena.php?modulo=principal/povosIndigenas&acao=I');

        }
   }

     


include APPRAIZ . 'includes/cabecalho.inc';
?>
<br>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Povos Indígenas</label></td></tr><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td></tr></table>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadPovoIndigena">
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         
            <tr>
                <td class ="SubTituloDireita" align"right"> Povo Indígena: </td>
                <td> 
                     <?php
                    $povnome = $rsDadosPovo[0]["povnome"];
                     ?>
                     <?= campo_texto('povnome', 'N', $somenteLeitura, '', 100, 600, '', '', 'left', '',  0, 'id="povnome" onblur="MouseBlur(this);"' ); ?>
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
                                    '<img onclick=\"return alterarPovoIndigena('|| povid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirPovoIndigena('|| povid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    povid, 
                                    povnome
                                FROM 
                                     parindigena.povoindigena";
                                     
                     if( strtoupper($_REQUEST['povnome']))
                     {     
                     $sql.=" WHERE 
                                    UPPER(povnome)  ILIKE '%" .strtoupper($_REQUEST['povnome']). "%' ";
                     }
                   
                     if( strtoupper($_REQUEST['povnome']))
                     {     
                     $sql.=" ORDER BY
                                    povnome ";
                     }               
                                    
                     $buscar = 1;
                }
                else
                {
                    $sql = "SELECT
                                    '<img onclick=\"return alterarPovoIndigena('|| povid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirPovoIndigena('|| povid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    povid,
                                    povnome
                                FROM 
                                     parindigena.povoindigena";  
                }
                $cabecalho = array( "Opções", "Código ", "Nome do Povo Indígena" );
                
                
                
        
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
    var total = cadPovoIndigena.elements.length;
    i = 0;
    while (i <= total)
     {
         if(cadPovoIndigena.elements[i].value == "")
         {
           alert("É necessário o preenchimento de todos os campos");
           
           cadPovoIndigena.elements[i].focus();
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
 function excluirPovoIndigena(povid)
         {
             if (confirm('Deseja excluir o registro?')) 
             {
                 return window.location.href = 'parindigena.php?modulo=principal/povosIndigenas&acao=I&excluir=' + povid;
                
             }
             
         }
         
function alterarPovoIndigena(povid)
{    
    return window.location.href = 'parindigena.php?modulo=principal/povosIndigenas&acao=I&alterar=' + povid;
}
 
 </script>

 
 
 
<?php
$db->executar("RESET CLIENT_ENCODING");

?>

