<?php

if(empty($_SESSION['obras2']['obrid']) && $_GET['obrid'] != ''){
    $_SESSION['obras2']['obrid'] = $_GET['obrid'];
    $obr = new Obras($_SESSION['obras2']['obrid']);
    $_SESSION['obras2']['empid'] = $obr->empid;
}
if($_SESSION['obras2']['orgid'] == ''){
    $_SESSION['obras2']['orgid'] = 3;
}
if($_SESSION['obras2']['empid'] == '' ){
    $obr = new Obras($_SESSION['obras2']['obrid']);
    $_SESSION['obras2']['empid'] = $obr->empid;
}
if($_SESSION['obras2']['empid'] == '' ){
    $_SESSION['obras2']['empid'] = $_SESSION['obras2']['obrid'];
}

if(!isset($_SESSION['esconde_btn_fechar_os_mi'])){
    $_SESSION['esconde_btn_fechar_os_mi'] = FALSE;
}

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

// empreendimento || obra || orgao
verificaSessao( 'obra' );

// Carrega dados das OS
$ordemServico = new OrdemServicoMI();
$dados_os_espelho = $ordemServico->getDadosEspelhoOsMi($obrid);

$os_exec = $dados_os_espelho['os_exec'];
$os_sond = $dados_os_espelho['os_sond'];
$os_impl = $dados_os_espelho['os_impl']; 

$_SESSION['tomid'] = (count($os_sond)>0)?2:((count($os_exec)>0)?1:((count($os_impl)>0)?3:0));

?>
<html>
    <head>
        <title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
    </head>
    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

    <body marginheight="0" marginwidth="0">
    <br/>
    <?php
    
        if(!$_SESSION['esconde_btn_fechar_os_mi']){
            echo monta_cabecalho_relatorio( '95' );
            $db->cria_aba(ID_ABA_OS_MI,$url,$parametros); 
            echo cabecalhoObra($obrid);
            monta_titulo( 'Dados das OS', '');
        }else{
            monta_titulo( 'Espelho das OS', '');            
        }
    ?>
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
        <tr>
            <td colspan="2" bgcolor="#DCDCDC">
                <!-- Sondagem - tomid = 2 -->
                <center><label class="TituloTela" style="color:#000000;">OS de Sondagem</label></center>
            </td>
        </tr>
        
        <?php
            if(count($os_sond) > 0){
        ?>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Tipo: </td>
                   <td> Sondagem </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Estado: </td>
                   <td> 
                       <?php
                           print_r('<b>'.$os_sond['estado_os'].'</b>');
                        ?> 
                   </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Data da OS: </td>
                   <td> 
                       <?php
                           $dt = explode(' ',$os_sond['osmdtcadastro']);
                           $dt = explode('-', $dt[0]);
                           echo $dt[2].'/'.$dt[1].'/'.$dt[0];
                        ?> 
                   </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Data de início da execução (cadastro): </td>
                   <td> 
                       <?php
                           if(!empty($os_sond['osmdtiniciomun'])) {
                               $dt = explode(' ', $os_sond['osmdtiniciomun']);
                               $dt = explode('-', $dt[0]);
                               echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                           }
                        ?> 
                   </td>
               </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%" valign="top"> Data de início da execução (aceite): </td>
                    <td>
                        <?php
                        if(!empty($os_sond['osmdtinicio'])) {
                            $dt = explode(' ', $os_sond['osmdtinicio']);
                            $dt = explode('-', $dt[0]);
                            echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                        }
                        ?>
                    </td>
                </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Prazo de execução (dias): </td>
                   <td>
                       <?php
                           print_r($os_sond['osmprazo']);
                        ?> 
                   </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Data de término da execução (cadastro): </td>
                   <td>
                       <?php
                        if(!empty($os_sond['osmdtterminomun'])) {
                            $dt = explode(' ', $os_sond['osmdtterminomun']);
                            $dt = explode('-', $dt[0]);
                            echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                        }
                        ?> 
                   </td>
               </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%" valign="top"> Data de término da execução (aceite): </td>
                    <td>
                        <?php
                        if(!empty($os_sond['osmdttermino'])) {
                            $dt = explode(' ', $os_sond['osmdttermino']);
                            $dt = explode('-', $dt[0]);
                            echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                        }
                        ?>
                    </td>
                </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Ordem de Serviço: </td>
                   <td>
                       <?php
                           print_r($os_sond['arquivo_os'][0]['link']);
                        ?>  
                   </td>
               </tr>
               <?php if(isset($os_sond['arq_os_ass'])){ ?>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Ordem de Serviço Assinada: </td>
                   <td> 
                       <?php
                           echo $os_sond['arq_os_ass'][0]['link'];
                        ?> 
                   </td>
               </tr>
               <?php } ?>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Laudo de Sondagem: </td>
                   <td>
                       <?php
                           print_r($os_sond['laudo_sondagem'][0]['link']);
                        ?>
                   </td>
               </tr>
            <?php    
            }else{
                echo '<tr>
                        <td colspan="2">
                            Não existe OS de Sondagem cadastrada.
                        </td>
                      </tr>';
            }
        ?>
                
        <tr>
            <td colspan="2" bgcolor="#DCDCDC">
                <!-- Projeto de Implantação - tomid = 3 -->
                <center><label class="TituloTela" style="color:#000000;">OS do Projeto de Implantação</label></center>
            </td>
        </tr>
        
        <?php
            if(count($os_impl) > 0){
        ?>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Tipo: </td>
                   <td> Projeto de Implantação </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Estado: </td>
                   <td> 
                       <?php
                           print_r('<b>'.$os_impl['estado_os'].'</b>');
                        ?> 
                   </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Data da OS: </td>
                   <td>
                       <?php
                           $dt = explode(' ', $os_impl['osmdtcadastro']);
                           $dt = explode('-', $dt[0]);
                           echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                        ?>
                   </td>
               </tr>
               <tr>
                    <td class="SubTituloDireita" width="20%" valign="top"> Data de início da execução (cadastro): </td>
                    <td>
                        <?php
                        if(!empty($os_impl['osmdtiniciomun'])) {
                            $dt = explode(' ', $os_impl['osmdtiniciomun']);
                            $dt = explode('-', $dt[0]);
                            echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                        }
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%" valign="top"> Data de início da execução (aceite): </td>
                    <td>
                        <?php
                        if(!empty($os_impl['osmdtinicio'])) {
                            $dt = explode(' ', $os_impl['osmdtinicio']);
                            $dt = explode('-', $dt[0]);
                            echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                        }
                        ?>
                    </td>
                </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Prazo de execução (dias): </td>
                   <td>
                       <?php
                           print_r($os_impl['osmprazo']);
                        ?>
                   </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Data de término da execução (cadastro): </td>
                   <td>
                       <?php
                           if(!empty($os_impl['osmdtterminomun'])) {
                               $dt = explode(' ', $os_impl['osmdtterminomun']);
                               $dt = explode('-', $dt[0]);
                               echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                           }
                        ?>
                   </td>
               </tr>
               <tr>
                    <td class="SubTituloDireita" width="20%" valign="top"> Data de término da execução (aceite): </td>
                    <td>
                        <?php
                        if(!empty($os_impl['osmdttermino'])) {
                            $dt = explode(' ', $os_impl['osmdttermino']);
                            $dt = explode('-', $dt[0]);
                            echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                        }
                        ?>
                    </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Ordem de Serviço: </td>
                   <td>
                       <?php
                           print_r($os_impl['arquivo_os'][0]['link']);
                        ?>
                   </td>
               </tr>
               <?php if(isset($os_impl['arq_os_ass'])){ ?>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Ordem de Serviço Assinada: </td>
                   <td> 
                       <?php
                           echo $os_impl['arq_os_ass'][0]['link'];
                        ?> 
                   </td>
               </tr>
               <?php } ?>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Anexos: </td>
                   <td>
                       <?php
                            foreach ($os_impl['anexos'] as $value) {
                                echo ' '.$value['link'].'<br>';
                            }
                        ?>
                   </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Serviços Externos: </td>
                   <td>
                       <table class="tabela listagem" style="text-align: center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
                            <thead>
                                <tr>
                                    <td><b>Item</b></td>
                                    <td><b>Valor Unitário</b></td>
                                    <td><b>Quantidade</b></td>
                                    <td><b>Unidade de medida</b></td>
                                    <td><b>Valor Total</b></td>
                                </tr>
                            </thead>
                            <tbody>
                                <?php
                                   $vl_tot = 0;
                                   foreach ($os_impl['servicos_externos'] as $servico) {
                                       echo '<tr>';
                                       echo '<td>'.$servico['item'].'</td>';
                                       echo '<td>'.$servico['valor_unitario'].'</td>';
                                       echo '<td>'.$servico['quantidade'].'</td>';
                                       echo '<td>'.$servico['unidade_medida'].'</td>';
                                       echo '<td>'.$servico['valor_total'].'</td>';
                                       echo '</tr>';
                                       
                                       $vl = str_replace('.', '', $servico['valor_total']);
                                       $vl = str_replace(',', '.', $vl);
                                       $vl = (float)$vl;
                                       $vl_tot += $vl;
                                   }
                                   
                                   echo ' 
                                        <tr>
                                            <th><b>TOTAL</b></th>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                            <td>'.number_format($vl_tot,2,',','.').'</td>
                                        </tr>  
                                        ';
                                ?>
                            </tbody>
                       </table>
                   </td>
               </tr>
            <?php    
            }else{
                echo '<tr>
                        <td colspan="2">
                            Não existe OS do Projeto de Implantação cadastrada.
                        </td>
                      </tr>';
            }
        ?>        
        <tr>
            <td colspan="2" bgcolor="#DCDCDC">
                <!-- Execução - tomid = 1 -->
                <center><label class="TituloTela" style="color:#000000;">OS de Execução</label></center>
            </td>
        </tr>        
        <?php
            if(count($os_exec) > 0){
        ?>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Tipo: </td>
                   <td> Execução </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Estado: </td>
                   <td> 
                       <?php
                           print_r('<b>'.$os_exec['estado_os'].'</b>');
                        ?> 
                   </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Data da OS: </td>
                   <td> 
                       <?php
                           $dt = explode(' ', $os_exec['osmdtcadastro']);
                           $dt = explode('-', $dt[0]);
                           echo $dt[2].'/'.$dt[1].'/'.$dt[0];
                        ?> 
                   </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Data de início da execução (cadastro): </td>
                   <td> 
                       <?php
                           if(!empty($os_exec['osmdtiniciomun'])) {
                               $dt = explode(' ', $os_exec['osmdtiniciomun']);
                               $dt = explode('-', $dt[0]);
                               echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                           }
                        ?> 
                   </td>
               </tr>
               <tr>
                    <td class="SubTituloDireita" width="20%" valign="top"> Data de início da execução (aceite): </td>
                    <td>
                        <?php
                        if(!empty($os_exec['osmdtinicio'])) {
                            $dt = explode(' ', $os_exec['osmdtinicio']);
                            $dt = explode('-', $dt[0]);
                            echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                        }
                        ?>
                    </td>
                </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Prazo de execução (dias): </td>
                   <td> 
                       <?php
                           print_r($os_exec['osmprazo']);
                        ?> 
                   </td>
               </tr>
               <tr>
                    <td class="SubTituloDireita" width="20%" valign="top"> Data de término da execução (cadastro): </td>
                    <td>
                        <?php
                        if(!empty($os_exec['osmdtterminomun'])) {
                            $dt = explode(' ', $os_exec['osmdtterminomun']);
                            $dt = explode('-', $dt[0]);
                            echo $dt[2] . '/' . $dt[1] . '/' . $dt[0];
                        }
                        ?>
                    </td>
               </tr>
               <tr>
                    <td class="SubTituloDireita" width="20%" valign="top"> Data de término da execução (aceite): </td>
                    <td>
                        <?php
                        if(!empty($os_exec['osmdttermino'])){
                            $dt = explode(' ', $os_exec['osmdttermino']);
                            $dt = explode('-', $dt[0]);
                            echo $dt[2].'/'.$dt[1].'/'.$dt[0];
                        }
                        ?>
                    </td>
               </tr>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Ordem de Serviço: </td>
                   <td> 
                       <?php
                           print_r($os_exec['arquivo_os'][0]['link']);
                        ?> 
                   </td>
               </tr>
               <?php if(isset($os_exec['arq_os_ass'])){ ?>
               <tr>
                   <td class="SubTituloDireita" width="20%" valign="top"> Ordem de Serviço Assinada: </td>
                   <td> 
                       <?php
                           echo $os_exec['arq_os_ass'][0]['link'];
                        ?> 
                   </td>
               </tr>
               <?php } ?>
            <?php    
            }else{
                echo '<tr>
                        <td colspan="2">
                            Não existe OS de Execução cadastrada.
                        </td>
                      </tr>';
            }
        ?>        
        <tr>
            <td colspan="2" bgcolor="#DCDCDC">
                &nbsp;
            </td>
        </tr>
        <?php
            if(!$_SESSION['esconde_btn_fechar_os_mi']){
        ?>
                <tr>
                    <td colspan="2" style="text-align:center;">
                        <input type="button" value="Fechar" style="cursor: pointer" onclick="window.close();">
                    </td>
                </tr>
        <?php
            }
            $_SESSION['esconde_btn_fechar_os_mi'] = false;
        ?>
    </table>
</body>
</html>