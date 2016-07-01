<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
	<tr bgcolor="#ffffff">
		<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
			SIMEC- Sistema Integrado do Ministério da Educação<br/>
			MEC / SE - Secretaria Executiva <br/>
		</td>
		<td></td>
	</tr>
	<tr>
		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
			Impresso por: <b> <?php echo $_SESSION['usunome'];?> </b><br/>
			Hora da Impressão: <?php echo date( 'd/m/Y - H:i:s' );?> <br />
		</td>
	</tr>
	<tr></tr>
		</table>
<?php
if($_SESSION['academico']['orgid'] == 2){
	print "<script>"
		. "    alert('Você não tem acesso a esta tela!');"
		. "    history.back(-1);"
		. "</script>";
	
	die;
}

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

$_SESSION["academico"]["tipocurso"]  = $_REQUEST["tpcid"] ? $_REQUEST["tpcid"] : 1;

print '<br/>';

// Monta as abas
monta_titulo( 'Extrato dos Cursos', '' );
?>
<br>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>
<script language="JavaScript">
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

<?php 

		$sql = "SELECT DISTINCT
					campus.emec,
					campus.descricao as campus,
					COALESCE(campus.prev2007,0) as prev2007, 
					COALESCE(campus.prev2008,0) as prev2008,
					COALESCE(campus.prev2009,0) as prev2009,
					COALESCE(campus.prev2010,0) as prev2010,
					COALESCE(campus.prev2011,0) as prev2011,
					COALESCE(campus.prev2012,0) as prev2012,
					SUM( COALESCE(campus.prev2007,0) + 
						COALESCE(campus.prev2008,0) +
						COALESCE(campus.prev2009,0) +
						COALESCE(campus.prev2010,0) +
						COALESCE(campus.prev2011,0) +
						COALESCE(campus.prev2012,0) ) as totalprev,
					campus.curid,
					campus.curso as cursofinal,
					campus.turprev
				FROM
					entidade.entidade e 
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				LEFT JOIN
					( SELECT
							e.entid AS codigo,
							e.entnome AS descricao,
							ea.entid AS instituicao,
							curd.cdtvgprevano2007 as prev2007,
							curd.cdtvgprevano2008 as prev2008,
							curd.cdtvgprevano2009 as prev2009,
							curd.cdtvgprevano2010 as prev2010,
							curd.cdtvgprevano2011 as prev2011,
							curd.cdtvgprevano2012 as prev2012,
							curd.cdtvgexecano2007 as exec2007,
							curd.cdtvgexecano2008 as exec2008,
							curd.cdtvgexecano2009 as exec2009,
							curd.cdtvgexecano2010 as exec2010,
							curd.cdtvgexecano2011 as exec2011,
							curd.cdtvgexecano2012 as exec2012,
							curd.cdtcodigoemec as emec,
							curso.curdsc as curso,
							curso.curid,
							te.turdsc AS turprev
						FROM
							entidade.entidade e
						INNER JOIN 
							entidade.funcaoentidade ef ON e.entid = ef.entid
						INNER JOIN 
							entidade.funentassoc ea ON ef.fueid = ea.fueid 
						LEFT JOIN
							academico.cursodetalhe curd ON curd.entid = e.entid
						LEFT JOIN 
							academico.turno te ON te.turid = curd.turidexecutado
						LEFT JOIN
							public.curso curso ON curd.curid = curso.curid
						WHERE 
							e.entstatus = 'A' 
							AND ef.funid = 18
						) as campus ON campus.instituicao = e.entid
				WHERE
					e.entstatus = 'A'
					AND e.entid = {$_REQUEST ['entid']}
					AND ef.funid  in ('12')
				GROUP BY
					campus.descricao,
					prev2007, prev2008, prev2009, prev2010, prev2011, prev2012,
					campus.curid,
					campus.curso,
					campus.turprev,
					campus.emec
				ORDER BY
					 cursofinal";
		
		
		//	$alinha = array("center","left","left","left","left","left","left");
		//	$tamanho = array("10%","10%","20%","20%","15%","15%","5%","5%");
		//	$db->monta_lista( $sql, array( "Ação", "Código e-MEC", "Nome", "Campus", "Programa", "Área", "Duração", "Turno" ), 30, 10, 'N', 'center', '','', $tamanho,$alinha);
		$dadoSql = $db->carregar( $sql );
		
		$dadoTd = 'align="right" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"';
		$dadoTd2 = 'align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"';
		
		?>
		<?php 
		//ver($dadoSql);
		$dadoSql = ($dadoSql) ? $dadoSql : array();
		foreach( $dadoSql as $dadoCurso ) { 
		monta_titulo( 'Curso', '' );?>
	  	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
		<thead>
		<tr bgcolor="#f5f5f5">
  			<td <?php echo $dadoTd2; ?> width="10%"><strong>Código e-MEC</strong></td>
  			<td <?php echo $dadoTd2; ?> width="20%"><strong>Nome</strong></td>
  			<td <?php echo $dadoTd2; ?> width="20%"><strong>Campus</strong></td>
  			<td <?php echo $dadoTd2; ?> width="5%"><strong>Turno</strong></td>
		</tr></thead>
		<tr bgcolor="#f5f5f5">
  			<td width="10%"><?php echo $dadoCurso['codemec']; ?></td>
  			<td width="20%"><?php echo $dadoCurso['cursofinal']; ?></td>
  			<td width="20%"><?php echo $dadoCurso['campus']; ?></td>
  			<td width="5%"><?php echo  $dadoCurso['turprev']; ?></td>
		</tr>
		<thead>
			<tr bgcolor="#f5f5f5">
		  	<td align="center" colspan='100%'><strong>Previsto:</strong></td>
		</tr></thead>
			<thead>
				<tr bgcolor="#f5f5f5"><td colspan='100%'>
				  	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2">
						<thead><tr colspan='100%' bgcolor="f5f5f5">
				  			<td <?php echo $dadoTd; ?>><strong>2007</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2008</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2009</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2010</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2011</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2012</strong></td>
						</thead></tr>
						<tr bgcolor="#f5f5f5">
				  			<td align="right"> <?php echo $dadosCurso['cdtvgprevano2007'] ; ?> </td>
				  			<td align="right"> <?php echo $dadosCurso['cdtvgprevano2008'] ; ?> </td>
				  			<td align="right"> <?php echo $dadosCurso['cdtvgprevano2009'] ; ?> </td>
				  			<td align="right"> <?php echo $dadosCurso['cdtvgprevano2010'] ; ?> </td>
				  			<td align="right"> <?php echo $dadosCurso['cdtvgprevano2011'] ; ?> </td>
				  			<td align="right"> <?php echo $dadosCurso['cdtvgprevano2012'] ; ?> </td>
						</tr>
				  	</table>
				</td></tr>
			</thead>
		<thead><tr bgcolor="#f5f5f5">
		  	<td align="center" colspan='100%'><strong>Executado no Próprio Curso:</strong></td>
		</tr></thead>
			<thead>
				<tr bgcolor="#f5f5f5"><td colspan='100%'>
				  	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2">
						<thead><tr colspan='100%' bgcolor="f5f5f5">
				  			<td <?php echo $dadoTd; ?>><strong>2007</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2008</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2009</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2010</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2011</strong></td>
				  			<td <?php echo $dadoTd; ?>><strong>2012</strong></td>
						</thead></tr>
						<tr bgcolor="#f5f5f5">
			  				<td align="right"> <?php echo $dadosCurso['cdtvgexecano2007'] ; ?> </td>
			  				<td align="right"> <?php echo $dadosCurso['cdtvgexecano2008'] ; ?> </td>
			  				<td align="right"> <?php echo $dadosCurso['cdtvgexecano2009'] ; ?> </td>
			  				<td align="right"> <?php echo $dadosCurso['cdtvgexecano2010'] ; ?> </td>
			  				<td align="right"> <?php echo $dadosCurso['cdtvgexecano2011'] ; ?> </td>
			  				<td align="right"> <?php echo $dadosCurso['cdtvgexecano2012'] ; ?> </td>
			  			</tr>
				  	</table>
				</td></tr>
			</thead>
		</table>
	  	<table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="2" align="center">
			<tr>
				<td class="subtitulocentro" colspan="2">Executado em Outro Curso</td><br>
			</tr>
		</table>
		<?php
	
			$sql = "SELECT DISTINCT
						cdtcodigoemec as codemec,
						curdsc as curso,
						curidpactuado,
						entnome as campus,
						cd.entid as entid,
						te.turdsc AS turprev,
						cdtid,
						cdtvgexecano2007, cdtvgexecano2008, cdtvgexecano2009, cdtvgexecano2010, cdtvgexecano2011, cdtvgexecano2012,
						cdtvgprevano2007, cdtvgprevano2008, cdtvgprevano2009, cdtvgprevano2010, cdtvgprevano2011, cdtvgprevano2012,
						SUM( COALESCE(cdtvgexecano2007,0) + 
							COALESCE(cdtvgexecano2008,0) +
							COALESCE(cdtvgexecano2009,0) +
							COALESCE(cdtvgexecano2010,0) +
							COALESCE(cdtvgexecano2011,0) +
							COALESCE(cdtvgexecano2012,0) ) as totalexec
					FROM 
						academico.cursoequivalencia eq
					INNER JOIN
						academico.cursodetalhe cd ON eq.curidexecutado = cd.curid
					INNER JOIN 
						public.curso c ON c.curid = cd.curid
					INNER JOIN
						entidade.entidade e ON e.entid = cd.entid
					LEFT JOIN
						academico.turno tp ON tp.turid = cd.turidprevisto
					LEFT JOIN 
						academico.turno te ON te.turid = cd.turidexecutado
					WHERE
						cdtpactuacao = 'E'
						AND cdtstatus = 'A'
						AND curidpactuado = {$dadoCurso['curid']}
					GROUP BY
						cdtcodigoemec,
						curdsc,
						curidpactuado,
						entnome,
						cd.entid,
						te.turdsc,
						cdtid,
						cdtvgexecano2007, cdtvgexecano2008, cdtvgexecano2009, cdtvgexecano2010, cdtvgexecano2011, cdtvgexecano2012,
						cdtvgprevano2007, cdtvgprevano2008, cdtvgprevano2009, cdtvgprevano2010, cdtvgprevano2011, cdtvgprevano2012
					ORDER BY
						curdsc";
			
	//		$alinha = array("center","left","left","left","left","left");
	//		$tamanho = array("10%","10%","20%","20%","20%","10%","10%");
	//		$db->monta_lista( $sql, array( "Ação", "Código e-MEC", "Nome do Curso", "Programa","Campus", "Turno", "Início" ), 30, 10, 'N', 'center', '','', $tamanho,$alinha);
	
			$dados = $db->carregar( $sql );
			$i = 1;
			
		?>
		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" >
		<thead><tr bgcolor="f5f5f5">
  			<td <?php echo $dadoTd2; ?> width="5%"><strong>Seq</strong></td>
  			<td <?php echo $dadoTd2; ?> width="10%"><strong>Código e-MEC</strong></td>
  			<td <?php echo $dadoTd2; ?> width="20%"><strong>Nome</strong></td>
  			<td <?php echo $dadoTd2; ?> width="20%"><strong>Campus</strong></td>
  			<td <?php echo $dadoTd2; ?> width="5%"><strong>Turno</strong></td>
		</tr></thead>
		</table>
		<center>
		<table width="95%" border="0" cellspacing="0" class="listagem" style="border-top: 0px">
			<?php if ($dados) { foreach( $dados as $dado ){ ?>
			<tr>
				<td rowspan='1' width="5%" align='center' bgcolor='#d0d0d0'><?php echo $i; ?></td>
				<td>
					<table width="100%">
						<thead>
						<tr align='center'  bgcolor="#f5f5f5">
				  			<td width="10%"><?php echo $dado['codemec']; ?></td>
				  			<td width="20%"><?php echo $dado['curso']; ?></td>
				  			<td width="21%"><?php echo $dado['campus']; ?></td>
				  			<td width="4%"><?php echo $dado['turprev']; ?></td>
						</tr>
						</thead>
						<?php if( $dado['cdtvgexecano2007'] <> 0 || $dado['cdtvgexecano2008'] <> 0 || $dado['cdtvgexecano2009'] <> 0 || $dado['cdtvgexecano2010'] <> 0 || $dado['cdtvgexecano2011'] <> 0 || $dado['cdtvgexecano2012'] <> 0 ) {?>
						<thead>
							<tr bgcolor="#f5f5f5">
								<td colspan='100%'>
								  	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2">
										<thead><tr colspan='100%' bgcolor="f5f5f5">
								  			<td <?php echo $dadoTd; ?>><strong>2007</strong></td>
								  			<td <?php echo $dadoTd; ?>><strong>2008</strong></td>
								  			<td <?php echo $dadoTd; ?>><strong>2009</strong></td>
								  			<td <?php echo $dadoTd; ?>><strong>2010</strong></td>
								  			<td <?php echo $dadoTd; ?>><strong>2011</strong></td>
								  			<td <?php echo $dadoTd; ?>><strong>2012</strong></td>
										</thead></tr>
										<tr bgcolor="#f5f5f5">
								  			<?php //echo campo_texto( $dado['cdtid'].'_cdtvgexecano2007', 'N', $habil2007, '', 6, 4, '####', '', '', '', '', '' ,'' , $dado['cdtvgexecano2007'], 'verifica( this.value, 2007, this.name );' ); ?>
								  			<td align="right"><?php echo $dado['cdtvgexecano2007']; ?></td>
								  			<td align="right"><?php echo $dado['cdtvgexecano2008']; ?></td>
								  			<td align="right"><?php echo $dado['cdtvgexecano2009']; ?></td>
								  			<td align="right"><?php echo $dado['cdtvgexecano2010']; ?></td>
								  			<td align="right"><?php echo $dado['cdtvgexecano2011']; ?></td>
								  			<td align="right"><?php echo $dado['cdtvgexecano2012']; ?></td>
										</tr>
								  	</table>
								</td>
							</tr>
						</thead>
						<?php } ?>
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
			}}?>
			</table>
		</div>
		</center>
		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2">
			<thead><tr colspan='100%' bgcolor="f5f5f5">
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
	  			<td align="right"> <?php echo $total2007 ; ?> </td>
	  			<td align="right"> <?php echo $total2008 ; ?> </td>
	  			<td align="right"> <?php echo $total2009 ; ?> </td>
	  			<td align="right"> <?php echo $total2010 ; ?> </td>
	  			<td align="right"> <?php echo $total2011 ; ?> </td>
	  			<td align="right"> <?php echo $total2012 ; ?> </td>
			</tr>
	  	</table>
	  	<br><br>
		<?php 
			$total2007 = '';
			$total2008 = '';
			$total2009 = '';
			$total2010 = '';
			$total2011 = '';
			$total2012 = '';
		} ?>
	  	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	</table>
</form>