<!--<script>-->
<!--    alert('O sistema mudou, clique no link para ir ao novo sistema!');-->
<!--</script>-->
<?php


include_once "config.inc";
include_once APPRAIZ."includes/classes_simec.inc";
include_once APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();
//include_once APPRAIZ."includes/cabecalho.php";

//Carrega parametros iniciais do simec
//include_once "controleInicio.inc";
//// carrega as funções específicas do módulo
//include_once '_constantes.php';
//include_once '_funcoes.php';
//include_once '_componentes.php';



/*
include APPRAIZ.'includes/cabecalho.inc';
		
echo "<html><body><form name=\"formulario\" method=\"post\" target=\"_blank\" action=\"http://pdeinterativo.mec.gov.br/login.php\">
      <input type=\"hidden\" name=\"formulario\" value=\"1\"/>
      <input  type=\"hidden\" name=\"usucpf\" value=\"".mascaraglobal($_SESSION['usucpf'],"###.###.###-##")."\"/>
      <input type=\"hidden\" name=\"ususenha\" value=\"".md5_decrypt_senha($db->pegaUm("SELECT ususenha FROM seguranca.usuario WHERE usucpf='".$_SESSION['usucpf']."'"),'')."\"/>
      <input type=\"hidden\" name=\"baselogin\" value=\"simec_espelho_producao\"/>
      <h1 align=center>Para melhoria da qualidade e velocidade dos serviços prestados,<br>a partir de agora você será redirecionado para o<br> novo endereço do PDEInterativo:<br><br><a style=\"cursor:pointer;\" onclick=\"document.formulario.submit();\"><font color='red'> http://pdeinterativo.mec.gov.br/</font></a></h1><br><br>
      <h1 align=center><a style=\"cursor:pointer;\" onclick=\"document.formulario.submit();\">Clique aqui para acessar!</h1>
      </form></body></html>";

include APPRAIZ.'includes/rodape.inc';
*/

//$perfis = pegaPerfilGeral();
/*
if(in_array(PDEESC_PERFIL_DIRETOR,$perfis) || 
   in_array(PDEINT_PERFIL_CONSULTA_ESTADUAL,$perfis) ||
   in_array(PDEINT_PERFIL_CONSULTA_MUNICIPAL,$perfis)) {
	include APPRAIZ.'includes/cabecalho.inc';
	echo "<br><br><h1><center>Em atendimento às solicitações encaminhadas pelas Secretarias, <br><font color=blue>o prazo final para envio do PDE Interativo para o MEC,<br> foi prorrogado para o dia 09/12/2011.</font><br><br> Nos dias 28, 29 e 30 de novembro de 2011, o PDE Interativo estará em manutenção, <br>visando dar maior agilidade ao sistema.</center></h1><br><br>";
	include APPRAIZ.'includes/rodape.inc';
} else {*/
//Carrega as funções de controle de acesso
//include_once "controleAcesso.inc";
//}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
?>
<table align="center" width="98%" border="0" class="tabela"  cellpadding="5" cellspacing="1" style="text-align: center;">
    <tr>
        <td style="font-size: 14px; font-weight: bold;">
            <p ><a style="color: red;" href="http://pddeinterativo.mec.gov.br" >Clique aqui para ir ao PDDE Interativo</a></p>
            <br />
            <p>Ou clique no nome do módulo atual que fica no canto superior à esquerda, caso queira ir para outro módulo do SiMEC.</p>
        </td>
    </tr>
</table>

