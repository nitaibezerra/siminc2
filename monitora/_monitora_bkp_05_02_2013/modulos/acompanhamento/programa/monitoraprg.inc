<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadprograma.inc
   Finalidade: permitir o controle do cadastro de programas
   */
$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";

?>
<title>Cadastro de Programas do Simec</title>

<body>
<? 
abas(5,'monitora.php?modulo=principal/programa/listaprograma','Listar programa','monitora.php?modulo=principal/programa/cadprograma','Atributo',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/cadindicador&prgcod=".$_REQUEST['prgcod'],'Indicadores',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/listaacao&prgcod=".$_REQUEST['prgcod'],'Ações',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/monitoraprg&prgcod=".$_REQUEST['prgcod'],'Acompanhamento');
abas(1,"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/monitoraprg&prgcod=".$_REQUEST['prgcod'],'Atributo','','Avaliação e Parecer',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/cadindicador&prgcod=".$_REQUEST['prgcod'],'Acompanhamento Físico',"<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/listaacao&prgcod=".$_REQUEST['prgcod'],'Acompanhamento Financeiro','','');
?>

<div align="center">
<table  width='100%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center'>
  <tr align="center">
    <td width="100%"><strong>Acompanhamento do Programa Escolha a Referência (<?=$nlinhas+1?>)</strong>
    </td>
  </tr>
</table>
<table  align='center'  width='100%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
  <tr>
    <td width="100%"><font color=red><strong><?print $_SESSION['msgsistema']; $_SESSION['msgsistema'] = '' ?></strong></font></td>
  </tr>
</table>
<table width='100%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"><strong>Código</strong></td>
      <td valign="top" class="title"><strong>Descrição</strong></td>
      <td valign="top" class="title"><strong>Ver ações</strong></td>
    </tr>
</thead>
<tbody>
<?
for ($i=0; $i<=$nlinhas;$i++){
  $res = $db->carrega_registro($RS,$i);
// a linha abaixo transforma em variáveis todos os campos do array
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
     print '<tr class="'.$marcado.'"><td><a href=simec?modulo=principal/programa/cadprograma&prgcod='.$codigo.'>'.$codigo.'</a></td><td>'.$descricao.'</td>';
     print '<td><a href=simec?modulo=principal/acao/listaacao&prgcod='.$codigo.'>Ver ações</a></td></tr>';
}
?>
</tbody>
</table>

      </td>
    </tr>
  </table>
  </center>
</div>