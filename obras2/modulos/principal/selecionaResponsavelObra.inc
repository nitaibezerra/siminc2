<?php
$obrid = $_SESSION['obras2']['obrid'];
$orgid = $_SESSION['obras2']['orgid'];

$obra 		= new Obras( $_SESSION['obras2']['obrid'] );
$endereco 	= new Endereco( $obra->endid );
$estuf		= $endereco->estuf;

$pflcod = $_POST['pflcod'];

if ( $orgid == ORGAO_FNDE ){
	$filtroCombo = " pflcod IN (" . PFLCOD_SUPERVISOR_UNIDADE . ", " . PFLCOD_GESTOR_UNIDADE . ", " . PFLCOD_EMPRESA_CONTRATADA . ")";
	$disabledPerfil = 'S';
}else{
	$filtroCombo = " pflcod IN (" . PFLCOD_EMPRESA_CONTRATADA . ")";
	$disabledPerfil = 'N';
}	


$filtro = Array();

if( $obra->entid ){
	$filtro[] = " ur.entid = ".$obra->entid;
}	

if( $orgid ){
	$filtro[] = " ur.orgid = ".$orgid;
}	

if( $estuf ){
	$filtro[] = " ur.estuf = '$estuf'";
}

$filtro = Array(" rpustatus = 'A' ", "(".implode(' OR ', $filtro).")");

function associaResponsavelObra( $dados ){

	$i = 0;

	if ( $dados["rpuid"] ){

		foreach( $dados["rpuid"] as $chave=>$valor ){
				
			$divisao = explode("_",$valor);
			$nome 	= $divisao[0];
			$cpf  	= $divisao[1];
			$pflcod = $divisao[2];
			$cpfcod = $cpf;
			$cpf 	= formatar_cpf($cpf);
			$botoes = '<img src="/imagens/excluir.gif" style="cursor: pointer"  border="0" title="Excluir" onclick="excluirResponsavel('.$chave.');"/>';
			$input  = '<input type="hidden" id="usucpf" name="usucpf['.$pflcod.']" value="'.$cpfcod.'"/>';
				
			$cor = ( $i % 2 ) ? "#e0e0e0" : "#F7F7F7";
				
			echo '<script type="text/javascript">
					var tabela = window.opener.document.getElementById("responsaveisobra");
					var tamanho = tabela.rows.length;
					var tr = tabela.insertRow(tamanho);
					tr.style.backgroundColor = \''.$cor.'\';
					tr.id = \'rpuid_'.$chave.'\';
					var colAcao = tr.insertCell(0);
					var colCPF  = tr.insertCell(1);
					var colNome = tr.insertCell(2);
					colAcao.style.textAlign = "center";
					colCPF.style.textAlign  = "center";
					colNome.style.textAlign  = "center";
					colAcao.innerHTML = \'' .$botoes . $input . '\';
					colCPF.innerHTML  = \''.$cpf.'\';
					colNome.innerHTML = \''.$nome.'\';
				</script>';
				
			$i++;
				
		}
	}

	echo '<script>window.close();</script>';

}

switch ($_REQUEST["requisicao"]) {

	case "pesquisa":
		if ( $_POST['pflcod'] )
			$filtro[] = " ur.pflcod = '" . $_POST['pflcod'] . "'";
		if ( $_POST['usucpf'] )
			$filtro[] = " ur.usucpf = '" . $_POST['usucpf'] . "'";
//		$filtro = pesquisaResponsavelObra( $_REQUEST );
	break;
	case "associa":
		 associaResponsavelObra( $_REQUEST );
	break;
	
}


print '<br/>';
monta_titulo( 'Usuário cadastrados no Módulo', '');

?>

<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <style>
			#div_rolagem table {
				width: 100%;                
			}
       </style>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form id="pesquisa" name="pesquisa" method="post" action="">
			<input type="hidden" name="requisicao" value="pesquisa"/>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulocentro" colspan="2">Filtros de Pesquisa</td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="170px">CPF</td>
					<td>
						<?php $usucpf = $_REQUEST["usucpf"]; ?>
						<?= campo_texto( 'usucpf', 'N', $somenteLeitura, '', 18, 15, '###.###.###-##', '', 'left', '', 0, 'id="usucpf"'); ?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Perfil</td>
					<td>
						<?php
							$pflcod = $orgid != ORGID_EDUCACAO_BASICA ? $_REQUEST["pflcod"] : PFLCOD_EMPRESA_CONTRATADA;
							
							$sql = "SELECT
										pflcod as codigo,
										pfldsc as descricao
									FROM
										seguranca.perfil
									WHERE
										sisid = {$_SESSION['sisid']} AND
										pflstatus = 'A' AND
										$filtroCombo";
							
							$db->monta_combo("pflcod", $sql, $disabledPerfil, "Selecione...", '', '', '', '', 'N', 'pflcod');
						?>
					</td>
				</tr>
				<tr bgcolor="#C0C0C0">
					<td></td>
					<td>
						<input type="button" style="cursor: pointer;" onclick="document.getElementById('pesquisa').submit();" value="Pesquisar"/>
					</td>
				</tr>
			</table>
		</form>
		<?php if( $pflcod != '' ){ ?>
		<form id="formulario" name="formulario" method="post" action="">
			<input type="hidden" name="requisicao" value="associa"/>
			<div id="div_rolagem" style="overflow-x: auto; overflow-y: auto; width:95%; height:180px; margin-left:2.5%	 ">
			<?php
				$sql = "SELECT DISTINCT
							'<center><input type=\"checkbox\" name=\"rpuid[' || rpuid || ']\" id=\"rpuid\" value=\"' || su.usunome || '_' || su.usucpf || '_' || ur.pflcod || '\" /></center>',
							su.usucpf, 
							'<center>'||su.usunome||'</center>' as nome
						FROM 
							seguranca.usuario su 
						INNER JOIN obras2.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf 
						WHERE 
							".(implode(' AND ',$filtro))."
						ORDER BY 
							su.usunome";
						
				$cabecalho = array( "Ação", "CPF", "Nome");
				$db->monta_lista_simples( $sql, $cabecalho, 10000, 30, 'N', '95%');
			?>
			</div>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr bgcolor="#C0C0C0">
					<td>
						<input type="button" style="cursor: pointer;" onclick="document.getElementById('formulario').submit();" value="Fechar"/>
					</td>
				</tr>
			</table>
		</form>
		<?php } ?>
	</body>
</html>