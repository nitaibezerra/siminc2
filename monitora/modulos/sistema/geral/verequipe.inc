<?
/*
Sistema Simec
Setor responsavel: SPO-MEC
Desenvolvedor: Equipe Consultores Simec
Analista: Gilberto Arruda Cerqueira Xavier
Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
Módulo:verequipe.inc
Finalidade: este arquivo permite ver a equipe do projeto e se comunicar com seus componentes
*/
 $pjeid = $_SESSION['pjeid'];

        $sql= "select ug.ungabrev ||'-'|| pe.pjecod || '  ' || pe.pjedsc as pjedsc from monitora.projetoespecial pe left join unidadegestora ug on ug.ungcod=pe.ungcod where pjeid=$pjeid";
		//print $sql;
        $pjedsc= $db->pegaUm($sql);        
        
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
  <tr>
    <td align='right' class="SubTituloDireita">Projeto :</td>
    <td><b><?=$pjedsc?></b></td>
  </tr>
<?
   $sql = "select distinct pfl.pflcod,pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra,usu.usucpf as membro, usu.usuemail, usu.usunome || ' ('|| ee.entnome ||')' as usuario,usu.usufoneddd||'-'||usu.usufonenum as fone from seguranca.perfil pfl left join monitora.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.pjeid = ".$_SESSION['pjeid']." and rpu.rpustatus='A' inner join seguranca.usuario usu on usu.usucpf=rpu.usucpf left join entidade.entidade ee on ee.entid = usu.entid where   pfl.pflstatus='A' and pfl.pflresponsabilidade in ('E') order by pfl.pflcod";

 $rs = @$db->carregar( $sql );
   if (  $rs && count($rs) > 0 )
	{
	 foreach ( $rs as $linha )
		{
		 foreach($linha as $k=>$v) ${$k}=$v;
            if ($usuario ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario.' Telefone:'.$fone;
	        if ($membro <> $_SESSION['usucpf'])
	        {
	        	$linha .= '&nbsp;&nbsp;&nbsp;<img src="../imagens/email.gif" title="Envia e-mail" border="0" onclick="envia_email(\''.$membro.'\');"> ';
	        }
	        $linha .= "</td></tr>";
            print $linha;
 
		}
	}
}
?>
</table>

<script >
	function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
</script>