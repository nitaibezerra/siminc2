<?

$coordlocal = carregarDadosIdentificacaoUsuario(
												array("picid"  => $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'],
													  "pflcod" => PFL_COORDENADORLOCAL,
													  "iusd"   => $_SESSION['sispacto3']['coordenadorlocal']['iusd']
													 ));

$mensagemalert	= "Dados do Cooordenador do Pacto salvos com sucesso";
$sis	  		= "coordenadorlocal";

if($coordlocal) {
	$coordlocal = current($coordlocal);
	extract($coordlocal);
}

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis)) {
	$consulta = true;
}


if($iusd) include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";
else echo "<p align=center>Coordenador Local não esta cadastrado. Por favor, solicite para o responsável (Equipe Municipal - Aprovação) deste município o cadastramento do Coordenador Local</p>";

?>