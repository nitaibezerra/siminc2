<?
include "_funcoes_professoralfabetizador.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

monta_titulo( "Lista - Coordenador Pedagógico", "Lista de Coordenador Pedagógico");

?>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">CPF</td>
	<td><?
	echo campo_texto('cpf', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, '', '', $_REQUEST['cpf'] );
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome</td>
	<td><?
	echo campo_texto('nome', "S", "S", "Nome", 50, 100, "", "", '', '', 0, '', '', $_REQUEST['nome'] );
	?></td>
</tr>
<tr>
   	<td class="SubTituloDireita" width="25%">Coordenadores Pedagógicos marcados como Orientadores de Estudo</td>
	<td><input type="checkbox" name="isorientadoresestudo" value="1" <?=(($_REQUEST['isorientadoresestudo'])?"checked":"") ?>></td>
</tr>
       <tr>
        	<td class="SubTituloDireita" width="25%">IES</td>
        	<td>
        	<?
        	$sql = "SELECT u.uncid as codigo, uu.unisigla||' - '||uu.uninome as descricao FROM sismedio.universidadecadastro u  
					INNER JOIN sismedio.universidade uu ON uu.uniid = u.uniid 
					ORDER BY uu.unisigla";
			$db->monta_combo('uncid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'uncid', false, $_REQUEST['uncid']);
        	
        	?>        	
        	</td>
        </tr>

<tr>
        	<td class="SubTituloDireita">UF</td>
        	<td>
        	<?
        	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
			$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF', '', '', '', 'N', 'estuf', false, $_REQUEST['estuf']);
        	?>        	
        	</td>
        </tr>
        <tr>
        	<td class="SubTituloDireita">Município</td>
        	<td id="td_municipio">
        	<?php 
        	if($_REQUEST['estuf']) {
        		carregarMunicipiosPorUF(array('id'=>'muncod_sim','name'=>'muncod','estuf'=>$_REQUEST['estuf'],'valuecombo'=>$_REQUEST['muncod']));
        	}
        	?>
        	</td>
        </tr>

<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" name="filtrar" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sismedio.php?modulo=principal/professoralfabetizador/listaprofessoralfabetizador&acao=A';"> <input type="submit" name="gerarxls" value="Gerar XLS"></td>
</tr>
</table>
</form>
<?

if($_REQUEST['filtrar'] || $_POST['gerarxls']) {

	if(!$_REQUEST['gerarxls']) $f[] = "foo.status='A' AND foo.status IS NOT NULL AND foo.uncid IS NOT NULL AND foo.perfil IS NOT NULL";
	
	if($_REQUEST['muncod']) {
		$f2[] = "i.muncodatuacao='".$_REQUEST['muncod']."'";
	}
	
	if($_REQUEST['estuf']) {
		$f2[] = "mu.estuf='".$_REQUEST['estuf']."'";
	}
	
	
	if($_REQUEST['cpf']) {
		$f[] = "foo.iuscpf='".str_replace(array(".","-"),array("",""),$_REQUEST['cpf'])."'";
	}
	
	if($_REQUEST['nome']) {
		$f[] = "foo.iusnome ilike '%".$_REQUEST['nome']."%'";
	}
	
	if($_REQUEST['isorientadoresestudo']) {
		$inn = "INNER JOIN sismedio.definicaoorientadoresestudo dd ON dd.iusd = i.iusd AND i.iuscodigoinep = dd.doecodigoinep";
	}
	
	if($_REQUEST['uncid']) {
		$f[] = "foo.uncid='".$_REQUEST['uncid']."'";
	}
	
	
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_COORDENADORIES,$perfis)) {
		if($_SESSION['sismedio']['universidade']['uncid']) {
			$f[] = "foo.uncid = '".$_SESSION['sismedio']['universidade']['uncid']."'";
		} else {
			$f[] = "1=2";
		}
	}
	
	if(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
		if($_SESSION['sismedio']['coordenadoradjuntoies']['uncid']) {
			$f[] = "foo.uncid = '".$_SESSION['sismedio']['coordenadoradjuntoies']['uncid']."'";
		} else {
			$f[] = "1=2";
		}
	}
	
	$sql = "SELECT 
				'<img src=\"../imagens/alterar.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"window.location=\'sismedio.php?modulo=principal/coordenadorpedagogico/coordenadorpedagogico&acao=A&iusd='||foo.iusd||'&uncid='||foo.uncid||'&requisicao=carregarCoordenadorPedagogico&direcionar=true\';\">' as acao,
				replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf,
				foo.iusnome,
				foo.iusemailprincipal,
				CASE WHEN foo.iustermocompromisso=true THEN 'Sim' ELSE 'Não' END as termo,
				foo.orientadornome
			FROM (
			SELECT
			i.iusd, 
			i.uncid, 
			i.iuscpf,
			i.iusnome,
			i.iusemailprincipal,
			i.iustermocompromisso, 
			i2.iusnome as orientadornome,
			(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_MEDIO.") as status,
			(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_COORDENADORPEDAGOGICO.") as perfil,
			i.muncodatuacao as muncod
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			{$inn}
			LEFT JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
			LEFT JOIN sismedio.turmas tt ON tt.turid = ot.turid 
			LEFT JOIN sismedio.identificacaousuario i2 ON i2.iusd = tt.iusd 
			LEFT JOIN territorios.municipio mu ON mu.muncod = i.muncodatuacao 
			WHERE i.iusstatus='A' AND t.pflcod='".PFL_COORDENADORPEDAGOGICO."' ".(($f2)?"AND ".implode(" AND ",$f2):"").") foo
			".(($f)?"WHERE ".implode(" AND ",$f):"")."
			ORDER BY foo.iusnome";
			
	$cabecalho = array("&nbsp;","CPF","Nome","E-mail","Termo aceito?","Orientador de Estudo");
	
	if($_POST['gerarxls']) {
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=rel_professor_".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=rel_orientador_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%','');
		exit;
	
	}
	
	$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);

}
?>