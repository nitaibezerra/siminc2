<?php
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);

if($_SESSION['academico']['orgid'] == 2){
	print "<script>"
		. "    alert('Você não tem acesso a esta tela!');"
		. "    history.back(-1);"
		. "</script>";
	
	die;
}

$perfilNotBloq = array(//PERFIL_IFESCADBOLSAS, 
					   //PERFIL_IFESCADCURSOS, 
					   //PERFIL_IFESCADASTRO, 
					   PERFIL_IFESCADTCU,
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);

$_SESSION['academico']['entidcampus'] = $_REQUEST['entidcampus'] ? $_REQUEST['entidcampus'] : $_SESSION['academico']['entidcampus'];
$entidCampus = $_SESSION['academico']['entidcampus'];
$entid	 	 = $_SESSION['academico']['entid'];

if( !$entid ){
	print "<script>"
		. "    alert('Sua sessão expirou!');"
		. "    history.back(-1);"
		. "</script>";
	die;
}

// A variável "$entidade" receberá o entid da instituição ou do campus, conforme o parametro $_REQUEST['acao'] 
if ( $_REQUEST['acao'] == 'C'){
	$entidade 	  = $entid;
	$tipoEntidade = 'instituição';
}else{
	$entidade 	  = $entidCampus;
	$tipoEntidade = 'campus';
}

if ($_REQUEST['deleta']){
	$ceqid 	= $_REQUEST['ceqid'];
	$sql = "DELETE FROM academico.cursoequivalencia WHERE ceqid = {$ceqid}";
	$db->executar($sql);
	$db->commit();
	die;
}

if ($_REQUEST['salvaVagas']){
	$arCampos = array('cdtvgexecano2007', 'cdtvgexecano2008', 'cdtvgexecano2009', 'cdtvgexecano2010', 'cdtvgexecano2011', 'cdtvgexecano2012' );
	$cdtid	  = $_REQUEST['cdtid'];
	foreach( $arCampos as $campo ){
		if( $_REQUEST[$campo] ){
	//		if( key_exists( $campo, $_REQUEST ) ){
			$valor = $_REQUEST[$campo];
			$campos.= $campo." = '".$valor."',";
		} else {
			$campos.= $campo." = null,";
		}
	}
	
	$campos = substr($campos, 0, -1);

	$sql = "UPDATE
				academico.cursodetalhe
			SET
				{$campos}
			WHERE
				cdtid = {$cdtid}";
	$db->executar($sql);	
	
	foreach( $_REQUEST as $chave => $values ){
		if(strstr($chave,"_cdtvgexecano"))
		{
			$pergunta = explode("_",$chave);
			if( $values ){
				$sql = "UPDATE
							academico.cursodetalhe
						SET
							{$pergunta[1]} = '{$values}'
						WHERE
							cdtid = {$pergunta[0]}";
				$db->executar($sql);
			} else {
				$sql = "UPDATE
							academico.cursodetalhe
						SET
							{$pergunta[1]} = null
						WHERE
							cdtid = {$pergunta[0]}";
				$db->executar($sql);
			}
		}
	}
	
	$db->commit();
	die;
}

$autoriazacaoconcursos = new autoriazacaoconcursos();
$c = new CursosEdital();

$_SESSION["academico"]["tipocurso"]  = $_REQUEST["tpcid"] ? $_REQUEST["tpcid"] : 1;

switch ($_REQUEST['evento']){
	
	case "excluir":
		$c->excluiCurso( $_REQUEST["cdtid"] );
	break;
	
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// Monta as abas
#$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Lista de Cursos', '' );



?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>
<script language="JavaScript">
	function excluiCurso(ceqid)
	{
		divCarregando();
		if (confirm('Tem certeza que deseja excluir essa relação?')){
			jQuery.ajax({
			   type: "POST",
			   url: "academico.php?modulo=principal/cursosevagas/listaCurso&acao=A",
			   data: "&deleta=true&ceqid="+ceqid,
			   success: function(msg){
			   		alert('Relação excluida com sucesso!');
					divCarregado();
			   		window.location.href = window.location.href;
			   }
			 });
		} else {
			divCarregado();
			return false;
	   }
	}
	
	function salvarVagas(){
		divCarregando();
		var dado = jQuery('#formCurso').serializeArray();
		dado.push({name  : 'salvaVagas',
				   value : 'true'});

		jQuery.ajax({
			   type: "POST",
			   url: "academico.php?modulo=principal/cursosevagas/listaCurso&acao=C",
			   data: dado,
			   async: false,
			   success: function(res){
			   		alert('Dados salvos com sucesso!');
					//alert( res );
					divCarregado();
			   		window.location.href = window.location.href;
			   }
			 });
	}
	
	function verifica(campo, ano, nome){
		if( ano == 2007 ){
			if(Number(campo) > Number($("input[name='cdtvgprevano2007']").val())){
				alert("O valor do campo não pode ser maior\nque o valor do previsto do curso.");
				$("input[name='"+nome+"']").val('');
				return false;
			}
		} else if( ano == 2008 ){
			if(Number(campo) > Number($("input[name='cdtvgprevano2008']").val())){
				alert("O valor do campo não pode ser maior\nque o valor do previsto do curso.");
				$("input[name='"+nome+"']").val('');
				return false;
			}
		} else if( ano == 2009 ){
			if(Number(campo) > Number($("input[name='cdtvgprevano2009']").val())){
				alert("O valor do campo não pode ser maior\nque o valor do previsto do curso.");
				$("input[name='"+nome+"']").val('');
				return false;
			}
		} else if( ano == 2010 ){
			if(Number(campo) > Number($("input[name='cdtvgprevano2010']").val())){
				alert("O valor do campo não pode ser maior\nque o valor do previsto do curso.");
				$("input[name='"+nome+"']").val('');
				return false;
			}
		} else if( ano == 2011 ){
			if(Number(campo) > Number($("input[name='cdtvgprevano2011']").val())){
				alert("O valor do campo não pode ser maior\nque o valor do previsto do curso.");
				$("input[name='"+nome+"']").val('');
				return false;
			}
		} else if( ano == 2012 ){
			if(Number(campo) > Number($("input[name='cdtvgprevano2012']").val())){
				alert("O valor do campo não pode ser maior\nque o valor do previsto do curso.");
				$("input[name='"+nome+"']").val('');
				return false;
			}
		}
	}
</script>
<div class='col-md-2' style='position:static;'>
        <?=cria_aba_2($abacod_tela, $url, $parametros);?>
    </div>
<div class='col-md-9' style='position:static;'>
    <?php
    // cria o cabeçalho padrão do módulo
    echo $autoriazacaoconcursos->cabecalho_entidade($entidade);
    ?>
<form id="formCurso" name="formCurso" action="" method="post">
	<input type="hidden" value="<?=$entidCampus ?>" name="hidEntidade" id="hidEntidade">
	<input type="hidden" value="<?=$tipocurso ?>" name="hidTipoCurso" id="hidTipoCurso">
	<input type="hidden" value="<?=$_REQUEST['cdtid'] ?>" name="cdtid" id="cdtid">
	<table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtitulocentro" colspan="2">Argumentos de Pesquisa</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="190px;">Nome</td>
			<td>
				<?php $curdsc = $_REQUEST["curdsc"]; ?>
				<?=campo_texto('curdsc','N','S','',60,100,'','', 'left', '', 0, 'id="curdsc"');?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Campus</td>
			<td>
				<?

					$entidcampus = $_REQUEST["entidcampus"];
				
					$sql = "SELECT
								e.entid AS codigo,
								e.entnome AS descricao
							FROM
								entidade.entidade e2
							INNER JOIN 
								entidade.entidade e ON e2.entid = e.entid
							INNER JOIN 
								entidade.funcaoentidade ef ON e.entid = ef.entid
							INNER JOIN 
								entidade.funentassoc ea ON ef.fueid = ea.fueid 
							-- INNER JOIN 
							--	academico.entidadeportaria ep ON ea.entid = ep.entid 
							-- INNER JOIN 
							--	academico.lancamentosportaria lp ON e.entid = lp.entidcampus 
							WHERE 
								e.entstatus = 'A' 
								AND ea.entid = " . $entid . "
								AND ef.funid = 18 
							GROUP BY 
								e.entid, 
								e.entnome";

					$db->monta_combo("entidcampus",$sql, 'S','Todos', '', '', '', 250, 'N','entidcampus', '', '', 'Campus');
				
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Situação do Curso</td>
			<td>
				<?
					$stcid = $_REQUEST["stcid"];
					
					$sql = "SELECT 
								stcid as codigo, 
								stcdsc as descricao 
							FROM 
								academico.situacaocurso
							order by stcdsc";
					
					$db->monta_combo("stcid",$sql, 'S','Todos', '', '', '', 250, 'N','stcid', '', '', 'Situação do Curso');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Programa</td>
			<td>
				<?
					$pgcid = $_REQUEST["pgcid"];
					
					$sql = "SELECT 
							  pgcid as codigo,
							  pgcdsc as descricao
							FROM 
							  academico.programacurso
							order by pgcdsc";
					
					$db->monta_combo("pgcid",$sql, 'S','Todos', '', '', '', 250, 'N','pgcid');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Área</td>
			<td>
				<?
					$arcid = $_REQUEST["arcid"];
					
					$sql = "SELECT 
							  arcid as codigo,
							  arcdsc as descricao
							FROM 
							  academico.areacurso
							order by arcdsc";
					
					$db->monta_combo("arcid",$sql, 'S','Todos', '', '', '', 250, 'N','arcid'); 
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<input type="button" value="Pesquisar" style="cursor:pointer;" onclick="document.getElementById('formCurso').submit();"/>
			</td>
		</tr>
	</table>

<br/>

<?php 

echo acaMenusTipoCurso( $_REQUEST["acao"], $_SESSION["academico"]["tipocurso"], "listaCurso" );
if( $_REQUEST["tpcid"] <> 3 ){
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulocentro" colspan="2">Lista de Cursos</td>
	</tr>
</table>
 
<?php

	// Lista de cursos
	$c->listaCursos( array("cdtpactuacao" => 'P'), array("tipoEntidade" => $tipoEntidade), $_REQUEST );
	if( academico_possui_perfil(PERFIL_ADMINISTRADOR) ){
	?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			    <tr bgcolor="#D0D0D0">
			        <td>
		        		<input type="button" value="Inserir Curso" style="cursor:pointer;" onclick="janela('?modulo=principal/cursosevagas/cadCurso&acao=A&tpcid=<?php echo $_SESSION["academico"]["tipocurso"]; ?>&entidunidade=<?php echo $entidade; ?>', 600, 480, 'cadCurso');">
			        	<input type="button" value="Extrato" style="cursor:pointer;" onclick="janela('?modulo=principal/cursosevagas/extratoCurso&acao=C&entid=<?php echo $entid?>', 600, 480, 'cadCurso');">
			        </td>
			    </tr>
		</table>
		
	<?php
	}
	else{
	?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			    <tr bgcolor="#D0D0D0">
			        <td>
		        		<input type="button" value="Extrato" style="cursor:pointer;" onclick="janela('?modulo=principal/cursosevagas/cadCurso&acao=A&tpcid=<?php echo $_SESSION["academico"]["tipocurso"]; ?>&entidunidade=<?php echo $entidade; ?>', 600, 480, 'cadCurso');">
			        </td>
			    </tr>
		</table>
		
	<?php
	}
	//dados do curso!
} else { 
	
	if( $_REQUEST['cdtid'] ){
		$dadosCurso = acaDadosCurso( $_REQUEST['cdtid'] );
		
	} else {
		print "<script>"
			. "    alert('Falta código do curso detalhe!');"
			. "    history.back(-1);"
			. "</script>";
		die;
	}

		$sql = "SELECT
					'<center><img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" border=0 title=\"Alterar Curso\" onclick=\"janela(\'?modulo=principal/cursosevagas/cadCurso&acao=A&cdtid=' || cdtid || '&curid=' || c.curid || '\', 600, 480, \'cadCurso\');\">' as acao,
					cdtcodigoemec as codemec,
					curdsc as curso,
					entnome as campus,
					pgcdsc as programa,
					arcdsc as area,
					cdtduracao as duracao,
					te.turdsc AS turprev			
				FROM 
				    academico.cursodetalhe cd
				INNER JOIN 
					public.curso c ON c.curid = cd.curid
				INNER JOIN
					entidade.entidade e ON e.entid = cd.entid
				INNER JOIN
					academico.areacurso ar ON ar.arcid = cd.arcid
				LEFT JOIN 
					academico.programacurso pc ON pc.pgcid = cd.pgcid
				LEFT JOIN
					academico.turno tp ON tp.turid = cd.turidprevisto
				LEFT JOIN 
					academico.turno te ON te.turid = cd.turidexecutado
				LEFT JOIN 
					academico.situacaocurso sc ON sc.stcid = cd.stcid
				WHERE
					cdtstatus = 'A' AND
					cdtid = ".$_REQUEST['cdtid'];
		
		//	$alinha = array("center","left","left","left","left","left","left");
		//	$tamanho = array("10%","10%","20%","20%","15%","15%","5%","5%");
		//	$db->monta_lista( $sql, array( "Ação", "Código e-MEC", "Nome", "Campus", "Programa", "Área", "Duração", "Turno" ), 30, 10, 'N', 'center', '','', $tamanho,$alinha);
		$dadoCurso = $db->pegaLinha( $sql );
		
		$dadoTd = 'align="right" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"';
		$dadoTd2 = 'align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"';
		
		?>
	  	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
		<thead><tr>
  			<td <?php echo $dadoTd2; ?> width="10%"><strong>Ação</strong></td>
  			<td <?php echo $dadoTd2; ?> width="10%"><strong>Código e-MEC</strong></td>
  			<td <?php echo $dadoTd2; ?> width="20%"><strong>Nome</strong></td>
  			<td <?php echo $dadoTd2; ?> width="20%"><strong>Campus</strong></td>
  			<td <?php echo $dadoTd2; ?> width="15%"><strong>Programa</strong></td>
  			<td <?php echo $dadoTd2; ?> width="15%"><strong>Área</strong></td>
  			<td <?php echo $dadoTd2; ?> width="5%"><strong>Duração</strong></td>
  			<td <?php echo $dadoTd2; ?> width="5%"><strong>Turno</strong></td>
		</tr></thead>
		<tr bgcolor="#f5f5f5">
  			<td width="10%"><?php echo $dadoCurso['acao']; ?></td>
  			<td width="10%"><?php echo $dadoCurso['codemec']; ?></td>
  			<td width="20%"><?php echo $dadoCurso['curso']; ?></td>
  			<td width="20%"><?php echo $dadoCurso['campus']; ?></td>
  			<td width="15%"><?php echo $dadoCurso['programa']; ?></td>
  			<td width="15%"><?php echo $dadoCurso['area']; ?></td>
  			<td width="5%"><?php echo  $dadoCurso['duracao']; ?></td>
  			<td width="5%"><?php echo  $dadoCurso['turprev']; ?></td>
		</tr>
		<thead><tr bgcolor="#f5f5f5">
		  	<td align="center" colspan='100%'><strong>Previsto:</strong></td>
		</tr></thead>
			<thead>
				<tr bgcolor="#f5f5f5"><td colspan='100%'>
				  	<table width="50%" align="center" border="0" cellspacing="0" cellpadding="2">
						<thead><tr colspan='100%'>
				  			<td <?php echo $dadoTd; ?>><strong>2007</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2008</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2009</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2010</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2011</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2012</strong></td>
						</thead></tr>
						<tr bgcolor="#f5f5f5">
				  			<td align="right"> <?php echo campo_texto( 'cdtvgprevano2007' , 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgprevano2007'] ); ?> </td>
				  			<td align="right"> <?php echo campo_texto( 'cdtvgprevano2008' , 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgprevano2008'] ); ?> </td>
				  			<td align="right"> <?php echo campo_texto( 'cdtvgprevano2009' , 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgprevano2009'] ); ?> </td>
				  			<td align="right"> <?php echo campo_texto( 'cdtvgprevano2010' , 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgprevano2010'] ); ?> </td>
				  			<td align="right"> <?php echo campo_texto( 'cdtvgprevano2011' , 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgprevano2011'] ); ?> </td>
				  			<td align="right"> <?php echo campo_texto( 'cdtvgprevano2012' , 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgprevano2012'] ); ?> </td>
						</tr>
				  	</table>
				</td></tr>
			</thead>
		<thead><tr bgcolor="#f5f5f5">
		  	<td align="center" colspan='100%'><strong>Execução no Próprio Curso:</strong></td>
		</tr></thead>
			<thead>
				<tr bgcolor="#f5f5f5"><td colspan='100%'>
				  	<table width="50%" align="center" border="0" cellspacing="0" cellpadding="2">
						<thead><tr colspan='100%'>
				  			<td <?php echo $dadoTd; ?>><strong>2007</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2008</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2009</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2010</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2011</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2012</strong></td>
						</thead></tr>
						<tr bgcolor="#f5f5f5">
			  				<td align="right"> <?php echo campo_texto( 'cdtvgexecano2007' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgexecano2007'] ); ?> </td>
			  				<td align="right"> <?php echo campo_texto( 'cdtvgexecano2008' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgexecano2008'] ); ?> </td>
			  				<td align="right"> <?php echo campo_texto( 'cdtvgexecano2009' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgexecano2009'] ); ?> </td>
			  				<td align="right"> <?php echo campo_texto( 'cdtvgexecano2010' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgexecano2010'] ); ?> </td>
			  				<td align="right"> <?php echo campo_texto( 'cdtvgexecano2011' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgexecano2011'] ); ?> </td>
			  				<td align="right"> <?php echo campo_texto( 'cdtvgexecano2012' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dadosCurso['cdtvgexecano2012'] ); ?> </td>
			  			</tr>
				  	</table>
				</td></tr>
			</thead>
		</table>
	  	<table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="2" align="center">
			<tr>
				<td class="subtitulocentro" colspan="2">Execução em Outro Curso</td>
			</tr>
			<tr>
				<td align="center" style="color:#cc0000;">Caso a execução do curso não tenha sido realizada de acordo com o previsto, informe abaixo o(s) curso(s) executado(s).</td>
			</tr>
		</table>
		<?php
	
			$sql = "SELECT DISTINCT
						'<center><img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" border=0 title=\"Alterar Curso\" onclick=\"janela(\'?modulo=principal/cursosevagas/cadCurso&acao=A&cdtid=' || cdtid || '&curid=' || eq.curidexecutado || '\', 600, 480, \'cadCurso\');\">&nbsp;		
						<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" border=0 title=\"Excluir Curso\" onclick=\"excluiCurso('||  eq.ceqid ||');\"></center>' as acao,
						cdtcodigoemec as codemec,
						curdsc as curso,
						entnome as campus,
						cd.entid as entid,
						pgcdsc as programa,
						arcdsc as area,
						cdtduracao as duracao,
						te.turdsc AS turprev,
						cdtid,
						cdtvgexecano2007, cdtvgexecano2008, cdtvgexecano2009, cdtvgexecano2010, cdtvgexecano2011, cdtvgexecano2012	
					FROM 
						academico.cursoequivalencia eq
					INNER JOIN
						academico.cursodetalhe cd ON eq.curidexecutado = cd.curid
					INNER JOIN 
						public.curso c ON c.curid = cd.curid
					INNER JOIN
						entidade.entidade e ON e.entid = cd.entid
					INNER JOIN
						academico.areacurso ar ON ar.arcid = cd.arcid
					LEFT JOIN 
						academico.programacurso pc ON pc.pgcid = cd.pgcid
					LEFT JOIN
						academico.turno tp ON tp.turid = cd.turidprevisto
					LEFT JOIN 
						academico.turno te ON te.turid = cd.turidexecutado
					WHERE
						curidpactuado = ".$dadosCurso['curid']."
						AND cdtpactuacao = 'E'
						AND cdtstatus = 'A'
					ORDER BY
						curdsc";

	//		$alinha = array("center","left","left","left","left","left");
	//		$tamanho = array("10%","10%","20%","20%","20%","10%","10%");
	//		$db->monta_lista( $sql, array( "Ação", "Código e-MEC", "Nome do Curso", "Programa","Campus", "Turno", "Início" ), 30, 10, 'N', 'center', '','', $tamanho,$alinha);
	
			$dados = $db->carregar( $sql );
			$i = 1;
			
			$habil2007 = $dadosCurso['cdtvgexecano2007'] ? 'N' : 'S';
			$habil2008 = $dadosCurso['cdtvgexecano2008'] ? 'N' : 'S';
			$habil2009 = $dadosCurso['cdtvgexecano2009'] ? 'N' : 'S';
			$habil2010 = $dadosCurso['cdtvgexecano2010'] ? 'N' : 'S';
			$habil2011 = $dadosCurso['cdtvgexecano2011'] ? 'N' : 'S';
			$habil2012 = $dadosCurso['cdtvgexecano2012'] ? 'N' : 'S';
			
		?>
		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
		<thead><tr>
  			<td <?php echo $dadoTd2; ?> width="5%"><strong>Seq</strong></td>
  			<td <?php echo $dadoTd2; ?> width="5%"><strong>Ação</strong></td>
  			<td <?php echo $dadoTd2; ?> width="10%"><strong>Código e-MEC</strong></td>
  			<td <?php echo $dadoTd2; ?> width="20%"><strong>Nome</strong></td>
  			<td <?php echo $dadoTd2; ?> width="20%"><strong>Campus</strong></td>
  			<td <?php echo $dadoTd2; ?> width="15%"><strong>Programa</strong></td>
  			<td <?php echo $dadoTd2; ?> width="15%"><strong>Área</strong></td>
  			<td <?php echo $dadoTd2; ?> width="5%"><strong>Duração</strong></td>
  			<td <?php echo $dadoTd2; ?> width="5%"><strong>Turno</strong></td>
		</tr></thead>
		</table>
		<center>
		<div id="divCursos" align="center" style="height:172px; width: 95%; overflow:auto">
			<table width="100%" border="0" cellspacing="0" class="listagem" style="border-top: 0px">
			<?php if ($dados) { foreach( $dados as $dado ){ ?>
			<tr>
				<td rowspan='1' width="5%" align='center' bgcolor='#d0d0d0'><?php echo $i; ?></td>
				<td>
					<table width="100%">
						<thead>
						<tr bgcolor="#f5f5f5">
				  			<td width="5%"><?php echo $dado['acao']; ?></td>
				  			<td width="10%"><?php echo $dado['codemec']; ?></td>
				  			<td width="20%"><?php echo $dado['curso']; ?></td>
				  			<td width="21%"><?php echo $dado['campus']; ?></td>
				  			<td width="16%"><?php echo $dado['programa']; ?></td>
				  			<td width="16%"><?php echo $dado['area']; ?></td>
				  			<td width="5%"><?php echo $dado['duracao']; ?></td>
				  			<td width="4%"><?php echo $dado['turprev']; ?></td>
						</tr>
						</thead>
						<thead>
							<tr bgcolor="#f5f5f5">
								<td colspan='100%'>
								  	<table width="50%" align="center" border="0" cellspacing="0" cellpadding="2">
										<thead><tr colspan='100%' bgcolor="e3e3e3">
								  			<td <?php echo $dadoTd; ?>><strong>2007</strong></td>
								  			<td <?php echo $dadoTd; ?>><strong>2008</strong></td>
								  			<td <?php echo $dadoTd; ?>><strong>2009</strong></td>
								  			<td <?php echo $dadoTd; ?>><strong>2010</strong></td>
								  			<td <?php echo $dadoTd; ?>><strong>2011</strong></td>
								  			<td <?php echo $dadoTd; ?>><strong>2012</strong></td>
										</thead></tr>
										<tr bgcolor="#f5f5f5">
								  			<?php //echo campo_texto( $dado['cdtid'].'_cdtvgexecano2007', 'N', $habil2007, '', 6, 4, '####', '', '', '', '', '' ,'' , $dado['cdtvgexecano2007'], 'verifica( this.value, 2007, this.name );' ); ?>
								  			<td align="right"><img src="/imagens/editar_nome.gif" align="top" style="cursor:pointer;" border=0 title="Execução do Curso" onclick="janela('?modulo=principal/cursosevagas/cadCursoOferta&acao=A&entidCampus=<?=$dado['entid'] ?>&cdtid=<?=$dado['cdtid'] ?>&ano=2007', 600, 480, 'cadCurso');">&nbsp;<?php echo campo_texto( $dado['cdtid'].'_cdtvgexecano2007', 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dado['cdtvgexecano2007'] ); ?> </td>
								  			<td align="right"><img src="/imagens/editar_nome.gif" align="top" style="cursor:pointer;" border=0 title="Execução do Curso" onclick="janela('?modulo=principal/cursosevagas/cadCursoOferta&acao=A&entidCampus=<?=$dado['entid'] ?>&cdtid=<?=$dado['cdtid'] ?>&ano=2008', 600, 480, 'cadCurso');">&nbsp;<?php echo campo_texto( $dado['cdtid'].'_cdtvgexecano2008', 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dado['cdtvgexecano2008'] ); ?> </td>
								  			<td align="right"><img src="/imagens/editar_nome.gif" align="top" style="cursor:pointer;" border=0 title="Execução do Curso" onclick="janela('?modulo=principal/cursosevagas/cadCursoOferta&acao=A&entidCampus=<?=$dado['entid'] ?>&cdtid=<?=$dado['cdtid'] ?>&ano=2009', 600, 480, 'cadCurso');">&nbsp;<?php echo campo_texto( $dado['cdtid'].'_cdtvgexecano2009', 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dado['cdtvgexecano2009'] ); ?> </td>
								  			<td align="right"><img src="/imagens/editar_nome.gif" align="top" style="cursor:pointer;" border=0 title="Execução do Curso" onclick="janela('?modulo=principal/cursosevagas/cadCursoOferta&acao=A&entidCampus=<?=$dado['entid'] ?>&cdtid=<?=$dado['cdtid'] ?>&ano=2010', 600, 480, 'cadCurso');">&nbsp;<?php echo campo_texto( $dado['cdtid'].'_cdtvgexecano2010', 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dado['cdtvgexecano2010'] ); ?> </td>
								  			<td align="right"><img src="/imagens/editar_nome.gif" align="top" style="cursor:pointer;" border=0 title="Execução do Curso" onclick="janela('?modulo=principal/cursosevagas/cadCursoOferta&acao=A&entidCampus=<?=$dado['entid'] ?>&cdtid=<?=$dado['cdtid'] ?>&ano=2011', 600, 480, 'cadCurso');">&nbsp;<?php echo campo_texto( $dado['cdtid'].'_cdtvgexecano2011', 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dado['cdtvgexecano2011'] ); ?> </td>
								  			<td align="right"><img src="/imagens/editar_nome.gif" align="top" style="cursor:pointer;" border=0 title="Execução do Curso" onclick="janela('?modulo=principal/cursosevagas/cadCursoOferta&acao=A&entidCampus=<?=$dado['entid'] ?>&cdtid=<?=$dado['cdtid'] ?>&ano=2012', 600, 480, 'cadCurso');">&nbsp;<?php echo campo_texto( $dado['cdtid'].'_cdtvgexecano2012', 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , $dado['cdtvgexecano2012'] ); ?> </td>
										</tr>
								  	</table>
								</td>
							</tr>
						</thead>
					</table>
				</td>
			</tr>
			<?php 
				if( $i == 1 ){ ?>
					<script>$('#divCursos').css('height', '87px');;</script>
				<?php } else { ?>
					<script>$('#divCursos').css('height', '172px');;</script>
				<?php } 
				$total2007 = $total2007 + $dado['cdtvgexecano2007'];
				$total2008 = $total2008 + $dado['cdtvgexecano2008'];
				$total2009 = $total2009 + $dado['cdtvgexecano2009'];
				$total2010 = $total2010 + $dado['cdtvgexecano2010'];
				$total2011 = $total2011 + $dado['cdtvgexecano2011'];
				$total2012 = $total2012 + $dado['cdtvgexecano2012'];
				$i++;
			}} else {?>
				<script>$('#divCursos').css('height', '25px');;</script>
				<tr>
					<td align="center" style="color:#cc0000;">Não existem cursos vinculados.</td>
				</tr>
			<?php } ?>
			</table>
		</div>
		</center>
		<table width="50%" align="center" border="0" cellspacing="0" cellpadding="2">
			<thead><tr colspan='100%' bgcolor="e3e3e3">
	  			<td <?php echo $dadoTd; ?> width='5%'><strong>TOTAL</strong></td>
	  			<td <?php echo $dadoTd; ?>><strong>2007</strong></td>
	  			<td <?php echo $dadoTd; ?>><strong>2008</strong></td>
	  			<td <?php echo $dadoTd; ?>><strong>2009</strong></td>
	  			<td <?php echo $dadoTd; ?>><strong>2010</strong></td>
	  			<td <?php echo $dadoTd; ?>><strong>2011</strong></td>
	  			<td <?php echo $dadoTd; ?>><strong>2012</strong></td>
			</thead></tr>
			<tr>
	  			<td align="right"></td>
	  			<td align="right"> <?php echo campo_texto( 'total2007', 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $total2007 ); ?> </td>
	  			<td align="right"> <?php echo campo_texto( 'total2008', 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $total2008 ); ?> </td>
	  			<td align="right"> <?php echo campo_texto( 'total2009', 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $total2009 ); ?> </td>
	  			<td align="right"> <?php echo campo_texto( 'total2010', 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $total2010 ); ?> </td>
	  			<td align="right"> <?php echo campo_texto( 'total2011', 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $total2011 ); ?> </td>
	  			<td align="right"> <?php echo campo_texto( 'total2012', 'N', 'N', '', 6, 4, '####', '', '', '', '', '' ,'' , $total2012 ); ?> </td>
			</tr>
	  	</table>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		    <tr bgcolor="#D0D0D0">
		        <td>
	        		<input type="button" value="Inserir Curso" style="cursor:pointer;" onclick="janela('?modulo=principal/cursosevagas/cadCurso&acao=A&tpcid=<?php echo $_SESSION["academico"]["tipocurso"]; ?>&curid=<?php echo $dadosCurso['curid']; ?>', 600, 480, 'cadCurso');">
		        </td>
		    </tr>
		</table>
	  	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	  		<td colspan="2" class="SubTituloEsquerda">
		  		<center>
					<input name="voltar" type="button" value="Voltar" onclick="history.back(-1);">
					<? if($permissoes['gravar']) { ?>
						<input name="salvar" type="button" value="Salvar" onclick="salvarVagas();">
					<? } ?>
				</center>
			</td>
	  	</tr>	  
	</table>
 <?php
}
?>
</form>
</div>