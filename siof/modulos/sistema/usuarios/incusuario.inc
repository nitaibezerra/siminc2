<?
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Alexandre Soares Diniz
   Programador: Alexandre Soares Diniz
   Módulo:incusuario.inc
   Finalidade: include de dados especificos para cadastro de usuarios no sistema SIOF, para entrada de dados, usado no modulo de segurança
 */
?> 

  	<tr>
        <td align='right' class="SubTituloDireita" >Perfil desejado:</td>
        <td >
        <?
       // $sql="select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflstatus='A' and pflnivel >=".$_SESSION['usunivel']." and sisid=".$_SESSION['sisid']."order by 2";
         //$sql="select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflstatus='A' and sisid=".$codigo." order by descricao";
         //print $sql;
         //$db->monta_combo("pflcod",$sql,'S',"Selecione o perfil desejado",'','');?>
         <?php include APPRAIZ .'seguranca/modulos/sistema/usuario/incperfilusuario.inc'; ?>
          </td>
      </tr>