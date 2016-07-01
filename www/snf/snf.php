<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';


//include APPRAIZ.'includes/cabecalho.inc';
		
/*
echo "<html>";
echo "	<body>";
echo "  	<h1 align=center>";
echo "			Bem-vindo ao SINAFOR!";
echo "		</h1>";
echo "		<center><font size=\"4px\">";
echo "			Estamos realizando os últimos ajustes para que os planos das escolas sejam trabalhados pelas Secretarias de Educação de sua rede.";
echo "		</font></center>";
echo "		<center><font size=\"4px\">";
echo "			Por favor, retorne mais tarde.";
echo "		</font></center>";
echo "	</body>";
echo "</html>";
*/

//include APPRAIZ.'includes/rodape.inc';

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>