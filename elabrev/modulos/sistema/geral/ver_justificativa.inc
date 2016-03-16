<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:ver_justificativa.inc
   Finalidade: permitir escrever e enviar email
   */
 if ($_REQUEST['tp'])
 $sql="select pfajustificativa as justificativa from elabrev.proposta_fusao_acao where pfaid =".$_REQUEST['cod'];
 else   $sql="select erajustificativa as justificativa from elabrev.elaboracaorevisao where eraid =".$_REQUEST['cod'];
    $RSu = $db->record_set($sql);
    $resu =  $db->carrega_registro($RSu,0);
    if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;

?>
<html>
<head>
<title>Justificativa</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
    <center>
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
     <tr>
	 <td colspan="2" align="Center" bgcolor="#dedede">Justificativa</td>
	 </tr>
        <td colspan=2><?=campo_textarea('justificativa','N','N','','100%',25,'');?></td>
     </tr>
  </table>
</form> 
<script>
  function fechar_janela()
  {
    window.close();
  }
</script>
</body>
</html>
