<?php
ini_set("memory_limit","1024M");
set_time_limit(0);
/*** Inclue o cabeçalho do sistema e monta o título da página ***/
include APPRAIZ . "includes/cabecalho.inc";
echo "<br />";
monta_titulo("Escola", "Selecione o módulo desejado:");

if( $_REQUEST['exercicio'] ){
	$_SESSION["exercicio"] = $_REQUEST["exercicio"];
} else {
	$_SESSION["exercicio"] = $_SESSION["exercicio_atual"];
}

//ver($_REQUEST["exercicio"],$_SESSION["exercicio"],$_SESSION['exercicio_atual']);

?>

<style>
.box
{
	width: 30%;
	height: 80px;
	margin: 20px;
	float: left;
}

.box_titulo
{
	border: 1px solid rgb(136, 136, 136);
	width: 100%;
	background-color: rgb(238, 233, 233); 
	color: black; 
	font-weight: bold;
	text-align: center;
	padding-top: 5px;
	padding-bottom: 5px;
}

.box_corpo
{
	border-left: 1px solid rgb(136, 136, 136);
	border-right: 1px solid rgb(136, 136, 136);
	border-bottom: 1px solid rgb(136, 136, 136);
	cursor: pointer;
	width: 100%;
	text-align: center;
	background-color: #f5f5f5; 
	color: black;
	padding-top: 30px;
	padding-bottom: 30px;
}

a
{
	cursor:pointer;
}
</style>

<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script src="./js/meajax.js" type="text/javascript"></script>
<script src="./js/eaajax.js" type="text/javascript"></script>
<script src="./js/eabajax.js" type="text/javascript"></script>

<?php
	/*** Inicializa a variável que conterá os submódulos ***/
	$submodulos = "";
	/*** Recupera os perfis do usuário ***/
	$arrPerfil = arrayPerfil();

	/*** Super Usuário ***/
	if( in_array(PDEESC_PERFIL_SUPER_USUARIO, $arrPerfil) )
	{
		$escolaAcessivel = '';
		//if($_SESSION["exercicio"] != "2013"){
			$escolaAcessivel = '<div class="box"><div class="box_titulo">Escola Acessível</div><div class="box_corpo" onclick="window.location.href=\'/pdeescola/pdeescola.php?modulo=painel_escolaacessivel&acao=A\'"><a href="/pdeescola/pdeescola.php?modulo=painel_escolaacessivel&acao=A">Clique para acessar o módulo do Escola Acessível<br>&nbsp;</a></div></div>';
						
		//}
		
		/*** Permite acesso ao painel de qualquer submódulo do sistema ***/
		$submodulos =	 '<div class="box"><div class="box_titulo">PDE Escola</div><div class="box_corpo" onclick="window.location.href=\'/pdeescola/pdeescola.php?modulo=painel_pdeescola&acao=A\'"><a href="/pdeescola/pdeescola.php?modulo=painel_pdeescola&acao=A">Clique para acessar o módulo do PDE Escola<br>&nbsp;</a></div></div>'
						.'<div class="box"><div class="box_titulo">Mais Educação</div><div class="box_corpo" onclick="window.location.href=\'/pdeescola/pdeescola.php?modulo=painel_maiseducacao&acao=A\'"><a href="/pdeescola/pdeescola.php?modulo=painel_maiseducacao&acao=A">Clique para acessar o módulo do Mais Educação<br>&nbsp;</a></div></div>'	
						.$escolaAcessivel
						.'<div class="box"><div class="box_titulo">Escola Aberta</div><div class="box_corpo" onclick="window.location.href=\'/pdeescola/pdeescola.php?modulo=painel_escolaaberta&acao=A\'"><a href="/pdeescola/pdeescola.php?modulo=painel_escolaaberta&acao=A">Clique para acessar o módulo do Escola Aberta<br>&nbsp;</a></div></div>'
						.'<div class="box"><div class="box_titulo">Questionário SEESP</div><div class="box_corpo" onclick="window.location.href=\'/pdeescola/pdeescola.php?modulo=painel_questionarioSeesp&acao=A\'"><a href="/pdeescola/pdeescola.php?modulo=painel_questionarioSeesp&acao=A">Clique para acessar o módulo do Questionário SEESP<br>(Ações referentes à 2008)</a></div></div>';
	}
	/*** Se não for Super Usuário ***/
	else
	{
		/*** Recupera em quais submódulos o usuário tem perfil ***/
		$perfilPdeEscola		= possuiPerfilSubModulo('pdeescola', 		$arrPerfil);
		$perfilMaisEducacao		= possuiPerfilSubModulo('maiseducacao', 	$arrPerfil);
		$perfilEscolaAcessivel	= possuiPerfilSubModulo('escolaacessivel', 	$arrPerfil);
		$perfilEscolaAberta		= possuiPerfilSubModulo('escolaaberta', 	$arrPerfil);
		$perfilQuestSEESP		= possuiPerfilSubModulo('questionario', 	$arrPerfil);
		
		//ver($arrPerfil);
		//ver($perfilPdeEscola, $perfilMaisEducacao, $perfilEscolaAcessivel, $perfilEscolaAberta, $perfilQuestSEESP);
		/*** Possui perfil no PDE Escola ***/
		if( $perfilPdeEscola )
		{
			/*** Testa se o perfil do usuário permite visualização do painel (Administradores) ***/
			if( in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $arrPerfil) || in_array(PDEESC_PERFIL_CONSULTA, $arrPerfil) )
			{
				$link = '/pdeescola/pdeescola.php?modulo=painel_pdeescola&acao=A';
			}
			else
			{
				/*** Testa se o perfil do usuário permite acesso à lista ***/
				if( in_array(PDEESC_PERFIL_COMITE_ESTADUAL, $arrPerfil) || in_array(PDEESC_PERFIL_COMITE_MUNICIPAL, $arrPerfil) ||
				    in_array(PDEESC_PERFIL_MONITORAMENTO_ESTADUAL, $arrPerfil) || in_array(PDEESC_PERFIL_MONITORAMENTO_MUNICIPAL, $arrPerfil) )
				{
					$link = '/pdeescola/pdeescola.php?modulo=lista&acao=E';
				}
				else
				{
					$_SESSION['entid'] = pdeRecuperaResponsabilidadePerfil('entid');
					$link = '/pdeescola/pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A';
				}
			}
				
			if( in_array( PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $arrPerfil ) || in_array( PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $arrPerfil ) ){
				$submodulos .= '<div class="box"><div class="box_titulo">PDE Escola</div><div class="box_corpo" onclick="window.location.href=\''.$link.'\';"><a href="'.$link.'">Clique para acessar o módulo do PDE Escola<br>&nbsp;</a></div></div>';
			} else {
				/*** Testa se o usuário somente possui perfil no PDE Escola ***/
				if( !$perfilMaisEducacao && !$perfilEscolaAcessivel && !$perfilEscolaAberta && !$perfilQuestSEESP )
				{
					echo "<script>window.location.href = '".$link."';</script>";
					die;
				}
				else
				{
					$submodulos .= '<div class="box"><div class="box_titulo">PDE Escola</div><div class="box_corpo" onclick="window.location.href=\''.$link.'\';"><a href="'.$link.'">Clique para acessar o módulo do PDE Escola<br>&nbsp;</a></div></div>';
				}
			}
		}
		
		/*** Possui perfil no Mais Educação ***/
		if( $perfilMaisEducacao )
		{
			/*** Testa se o perfil do usuário permite visualização do painel (Administradores) ***/
			if( in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $arrPerfil) || in_array(PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO, $arrPerfil) )
			{
				$link = '/pdeescola/pdeescola.php?modulo=painel_maiseducacao&acao=A';
			}
			else
			{
				/*** Testa se o perfil do usuário permite acesso à lista ***/
				if( in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $arrPerfil) || in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $arrPerfil) )
				{
					unset($entid);
					$link = '/pdeescola/pdeescola.php?modulo=melista&acao=E';
				}
				else
				{
					/*** Se for cadastrador recupera a escola atribuída ao seu perfil e o redireciona ***/
					$entid 	= meRecuperaResponsabilidadePerfil('entid');
					$link	= "redirecionaME('meajax.php', 'tipo=redirecioname&entid=".$entid."');";
				}
			}
			
			/*** Testa se o usuário somente possui perfil no Mais Educação ***/
			if( !$perfilPdeEscola && !$perfilEscolaAcessivel && !$perfilEscolaAberta && !$perfilQuestSEESP )
			{
				if($entid)
				{
					echo "<script>".$link."</script>";
					die;
				}
				else
				{
					echo "<script>window.location.href = '".$link."';</script>";
					die;
				}
			}
			else
			{
				if($entid)
					$submodulos .= "<div class=\"box\"><div class=\"box_titulo\">Mais Educação</div><div class=\"box_corpo\" onclick=\"".$link."\"><a href=\"#\">Clique para acessar o módulo do Mais Educação<br>&nbsp;</a></div></div>";
				else
					$submodulos .= '<div class="box"><div class="box_titulo">Mais Educação</div><div class="box_corpo" onclick="window.location.href=\''.$link.'\';"><a href="'.$link.'">Clique para acessar o módulo do Mais Educação<br>&nbsp;</a></div></div>';
			}
		}
		
		/*** Possui perfil no Escola Acessível ***/
		
		if( $perfilEscolaAcessivel )
		{
			/*** Testa se o perfil do usuário permite visualização do painel (Administradores) ***/
			if( in_array(PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ACESSIVEL, $arrPerfil) || in_array(PDEESC_PERFIL_CONSULTA_ESCOLA_ACESSIVEL, $arrPerfil) )
			{
				$link = '/pdeescola/pdeescola.php?modulo=painel_escolaacessivel&acao=A';
			}
			else
			{
				/*** Testa se o perfil do usuário permite acesso à lista ***/
				if( in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL, $arrPerfil) || in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL, $arrPerfil) )
				{
					unset($entid);
					$link = '/pdeescola/pdeescola.php?modulo=ealista&acao=E';
				}
				else
				{
					/*** Se for cadastrador recupera a escola atribuída ao seu perfil e o redireciona ***/
					$entid 	= eaRecuperaResponsabilidadePerfil('entid');
					$link	= "redirecionaEA('eaajax.php', 'tipo=redirecionaea&entid=".$entid."');";
				}
			}
			
			/*** Testa se o usuário somente possui perfil no Escola Acessível ***/
			$dados = acessoCadEsc( $entid );
			if( $dados ){
				$perfilQuestSEESP = 1;
				$flag = 1;
			}
			if( !$perfilPdeEscola && !$perfilMaisEducacao && !$perfilEscolaAberta && !$perfilQuestSEESP )
			{ 
				if($entid)
				{
					echo "<script>".$link."</script>";
					die;
				}
				else
				{
					echo "<script>window.location.href = '".$link."';</script>";
					die;
				}
			}
			else
			{
				
				//if($_SESSION["exercicio"] != "2013" ){
				if($entid)
					$submodulos .= "<div class=\"box\"><div class=\"box_titulo\">Escola Acessível</div><div class=\"box_corpo\" onclick=\"".$link."\"><a href=\"#\">Clique para acessar o módulo do Escola Acessível<br>&nbsp;</a></div></div>";
				else
					$submodulos .= '<div class="box"><div class="box_titulo">Escola Acessível</div><div class="box_corpo" onclick="window.location.href=\''.$link.'\';"><a href="'.$link.'">Clique para acessar o módulo do Escola Acessível<br>&nbsp;</a></div></div>';
				//}
			}
		}
		
		/*** Possui perfil no Escola Aberta ***/
		if( $perfilEscolaAberta )
		{
		
			if( $_SESSION["exercicio"] == "2012" ){
				
				if( in_array(PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ABERTA, $arrPerfil) )
				{
					$link = '/pdeescola/pdeescola.php?modulo=painel_escolaaberta&acao=A';
					
					if($entid)
						$submodulos .= "<div class=\"box\"><div class=\"box_titulo\">Escola Aberta</div><div class=\"box_corpo\" onclick=\"".$link."\"><a href=\"#\">Clique para acessar o módulo do Escola Aberta<br>&nbsp;</a></div></div>";
					else
						$submodulos .= '<div class="box"><div class="box_titulo">Escola Aberta</div><div class="box_corpo" onclick="window.location.href=\''.$link.'\';"><a href="'.$link.'">Clique para acessar o módulo do Escola Aberta<br>&nbsp;</a></div></div>';				
				}
				
			}
			else{			
			
				/*** Testa se o perfil do usuário permite visualização do painel (Administradores) ***/
				if( in_array(PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ABERTA, $arrPerfil) || in_array(PDEESC_PERFIL_CONSULTA_ESCOLA_ABERTA, $arrPerfil) )
				{
					$link = '/pdeescola/pdeescola.php?modulo=painel_escolaaberta&acao=A';
				}
				else
				{
					/*** Testa se o perfil do usuário permite acesso à lista ***/
					if( in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ABERTA, $arrPerfil) || in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ABERTA, $arrPerfil) )
					{
						unset($entid);
						$link = '/pdeescola/pdeescola.php?modulo=eablista&acao=E';
					}
					else
					{
						/*** Se for cadastrador recupera a escola atribuída ao seu perfil e o redireciona ***/
						$entid 	= eabRecuperaResponsabilidadePerfil('entid');
						$link	= "redirecionaEAB('eabajax.php', 'tipo=redirecionaeab&entid=".$entid."');";
					}
				}
				
				/*** Testa se o usuário somente possui perfil no Escola Aberta ***/
				if( !$perfilPdeEscola && !$perfilMaisEducacao && !$perfilEscolaAcessivel && !$perfilQuestSEESP )
				{
					if($entid)
					{
						echo "<script>".$link."</script>";
						die;
					}
					else
					{
						echo "<script>window.location.href = '".$link."';</script>";
						die;
					}
				}
				else
				{
					if($entid)
						$submodulos .= "<div class=\"box\"><div class=\"box_titulo\">Escola Aberta</div><div class=\"box_corpo\" onclick=\"".$link."\"><a href=\"#\">Clique para acessar o módulo do Escola Aberta<br>&nbsp;</a></div></div>";
					else
						$submodulos .= '<div class="box"><div class="box_titulo">Escola Aberta</div><div class="box_corpo" onclick="window.location.href=\''.$link.'\';"><a href="'.$link.'">Clique para acessar o módulo do Escola Aberta<br>&nbsp;</a></div></div>';
				}
			
			}
		}
		
		if( $perfilQuestSEESP )
		{
			
			
			if( in_array(PDEESC_PERFIL_ESCOLA_QUEST_SEESP, $arrPerfil) ){
				$link = '/pdeescola/pdeescola.php?modulo=questionario&acao=A';
			} elseif( in_array(PDEESC_PERFIL_SEC_MUNICIPAL_QUEST_SEESP, $arrPerfil) ){
				$link = '/pdeescola/pdeescola.php?modulo=questionario&acao=B&aba=2';
			} elseif( in_array(PDEESC_PERFIL_SEC_ESTADUAL_QUEST_SEESP, $arrPerfil) ){
				$link = '/pdeescola/pdeescola.php?modulo=questionario&acao=C&aba=4';
			} elseif( in_array(PDEESC_PERFIL_ADM_QUEST_SEESP, $arrPerfil) ){
				$link = '/pdeescola/pdeescola.php?modulo=painel_questionarioSeesp&acao=A';
			}
			
			if( $flag == 1 ){
				$link = '/pdeescola/pdeescola.php?modulo=questionario&acao=A&aba=1';
			}

			/*** Testa se o usuário somente possui perfil no Questionário ***/
			if( !$perfilPdeEscola && !$perfilMaisEducacao && !$perfilEscolaAcessivel && !$perfilEscolaAberta )
			{
				if($entid)
				{
					echo "<script>".$link."</script>";
					die;
				}
				else
				{
					echo "<script>window.location.href = '".$link."';</script>";
					die;
				}
			} else {
				if($entid)
					$submodulos .= '<div class="box"><div class="box_titulo">Questionário SEESP</div><div class="box_corpo" onclick="window.location.href=\''.$link.'\';"><a href="'.$link.'">Clique para acessar o módulo do Questionário SEESP<br>(Ações referentes à 2008)</a></div></div>';
				else
					$submodulos .= '<div class="box"><div class="box_titulo">Questionário SEESP</div><div class="box_corpo" onclick="window.location.href=\''.$link.'\';"><a href="'.$link.'">Clique para acessar o módulo do Questionário SEESP<br>(Ações referentes à 2008)</a></div></div>';
			}
		}
		
	}
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="height:300px;">
	<tr>
		<td style="text-align:center;">
			<?=$submodulos?>
		</td>
	</tr>
</table>