<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:montageraca.inc
   Finalidade: permitir a montagem da lista
    */
    $RS = $db->record_set($sqlrel);
    $_SESSION['LINHA'] = $db->conta_linhas($RS);
    $_SESSION['title'] = $tit;
    $_SESSION['TITULO']= 'Relatório Gerencial de Ação';
    $_SESSION['TIT'][0][0] = 'Código ';
    $_SESSION['TIT'][0][1] = 'Título';
    $_SESSION['data']= date('(h:m) d/m/Y');
    $_SESSION['LIN'] = array();
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
    if ($_SESSION['LINHA'] >=0) print '<input type="button" class="botao" name="btrelat" value ="Gerar PDF" align="left" onclick="imprime_rel(1)">&nbsp;&nbsp;<a href="ar505ptb.exe"><img border="0" width="30" heigth="30" src="imagens/pdf_adobe.jpg" title=" Você não tem o Adobe Reader? Clique aqui!"></a>';
      print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar XLS" align="left" onclick="imprime_rel(4)">';
      print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar CSV" align="left" onclick="imprime_rel(6)">';      
    print '</td></tr></table>';

    $cabecalho = array('Ação','Código ','Título');
    $_SESSION['cab']=$cabecalho;
    $_SESSION['conteudo']=  $db->carrega_tudo($RS); 
    $db->monta_lista($sqlrel,$cabecalho,100,20,'','','');
    //

    if ($_SESSION['LINHA'] >= 0) {
        for ($i=0; $i<=$_SESSION['LINHA'];$i++){
            $res = $db->carrega_registro($RS,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
 	       $_SESSION['LIN'][$i][0] = $codigo;
		   $_SESSION['LIN'][$i][1] = $descricao;
        }
     }
     ?>
