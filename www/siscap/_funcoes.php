<?php

function montaListaCursos( $where = null ){

	global $db;

	$cabecalho = array("Ações", "Nome do Curso", "Área", "Turmas");
	$tamanho = array( '25%', '50%', '25%' );
	$alinhamento = array( 'center', 'left', 'center' );

	if( $where ){
		$w = "WHERE c.curdsc ILIKE '%".$where."%'";
	}

	$sql = "SELECT
				CASE WHEN
					(SELECT count(t.turid) FROM siscap.turma t WHERE t.curid = c.curid ) = 0
				THEN
					'<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarCurso('||c.curid||');\" border=0 alt=\"Ir\" title=\"Alterar\"></a> <img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirCurso('||c.curid||');\" border=0 alt=\"Ir\" title=\"Excluir\"> <img src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" onclick=\"visualizarCurso('||c.curid||');\" border=0 alt=\"Ir\" title=\"Visualizar\"></center>'
				ELSE
					'<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarCurso('||c.curid||');\" border=0 alt=\"Ir\" title=\"Alterar\"></a> <img src=\"/imagens/excluir_01.gif \" border=0 alt=\"Ir\" title=\"Existem turmas vinculadas a esse curso\"> <img src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" onclick=\"visualizarCurso('||c.curid||');\" border=0 alt=\"Ir\" title=\"Visualizar\">'
				END as acao,
				c.curdsc,
				ar.aredsc,
				(SELECT count(t.turid) FROM siscap.turma t WHERE t.curid = c.curid )
			FROM
				siscap.curso c
                inner join siscap.area ar on ar.areid = c.areid and ar.arestatus = 'A'
			{$w}
			ORDER BY
				c.curdsc";

		return $db->monta_lista( $sql, $cabecalho, 10, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
}

function montaListaFacilitador( $where = null ){

	global $db;

	$cabecalho = array("Ações", "Nome do Facilitador", "Situação");
	$tamanho = array( '25%', '75%');
	$alinhamento = array( 'center', 'center' );

	if( $where ){
		$w = "WHERE facnome ILIKE '%".$where."%'";
	}
	//<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirFacilitador('||facid||');\" border=0 alt=\"Ir\" title=\"Excluir\">
	$sql = "SELECT
				'<center>
					<img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarFacilitador('||fa.facid||');\" border=0 alt=\"Ir\" title=\"Alterar\"></a>
					<img src=\"/imagens/consultar.gif \" style=\"cursor: pointer\" onclick=\"visualizarFacilitador('||fa.facid||');\" border=0 alt=\"Ir\" title=\"Visualizar\"></a></center>' as acao,
				fa.facnome, fa.facid,
				(select dafid from siscap.dadosfuncionais where facid = fa.facid limit 1 ) as dafid,
                (select conid from siscap.contato where facid = fa.facid limit 1) as conid,
                (select forid from siscap.formacao where facid = fa.facid  limit 1) as forid,
                (select expid from siscap.experienciaprofissional where facid = fa.facid limit 1) as expid
			FROM
				siscap.facilitador fa
              	{$w}
			ORDER BY
				fa.facnome";

	$arDados = $db->carregar( $sql );
	$arDados = $arDados ? $arDados : array();

	$arRegistro = array();
	if(count($arDados)){
    	foreach ($arDados as $key => $v) {
    		$arStatus = array();

    		if( empty($v['dafid']) ){
    			$arStatus[] = "<a href='siscap.php?modulo=principal/dadosFuncionais&acao=A&facid=".$v['facid']."'><font color=\"red\">Dados Funcionais</font></a>";
    		}
    		if( empty($v['conid']) ){
    			$arStatus[] = "<a href='siscap.php?modulo=principal/contato&acao=A&facid=".$v['facid']."'><font color=\"red\">Contato</font></a>";
    		}
    		if( empty($v['forid']) ){
    			$arStatus[] = "<a href='siscap.php?modulo=principal/formacao&acao=A&facid=".$v['facid']."'><font color=\"red\">Formação</font></a>";
    		}
    		if( empty($v['expid']) ){
    			$arStatus[] = "<a href='siscap.php?modulo=principal/experienciaProfissional&acao=A&facid=".$v['facid']."'><font color=\"red\">Experiência Profisional</font></a>";
    		}
    		if( empty( $arStatus ) ){
    			$arStatus[] = 'Cadastro Concluído';
    			$result = implode( ', ', $arStatus);
    		}else{
        		$result = "Pendências: ".implode( ', ', $arStatus);
    		}

    		$arRegistro[$key] = array("acoes" => $v['acao'],
    								  	"facnome" => $v['facnome'],
    								  	"status" => $result
    								 );
    	}
	}

	unset($w);
	return $db->monta_lista_array($arRegistro, $cabecalho, 5000, 20, '', 'center', '');
	//return $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
}

function montaListaTelefone( $facid = null, $habilita = '' ){
	global $db;

	if( $facid ){
		$where = $facid;
	} else {
		$where = "0";
	}

	if( $habilita == 'N' ){
		$acoes = "'<center><img src=\"/imagens/excluir_01.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";
	} else {
		$acoes = "'<center><img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirTel('||telid||', '||facid||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";
	}

	$sql = "SELECT
				 $acoes as acao,
				'(' || telddd || ') ' as ddd, telnumero as numero, tp.ttedsc as tipo
			FROM
				siscap.telefone t
			INNER JOIN
				siscap.tipo_telefone tp ON tp.tteid = t.tteid
			WHERE
				facid = {$where}
			ORDER BY
				telid";
	$arDados = $db->carregar( $sql );
	$arDados = $arDados ? $arDados : array();

	$html = '';
	if( !empty($arDados) ){
		foreach ($arDados as $key => $v) {
			$key % 2 ? $cor = "#f7f7f7" : $cor = "";

			$telnumero = substr( $v['numero'], 0, 4).'-'.substr( $v['numero'], 4);

			$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td>'.$v['acao'].'</td>
					<td>'.$v['ddd'].$telnumero.'</td>
					<td>'.$v['tipo'].'</td>
				</tr>';
		}
	} else{
		$html.= '<tr><td align="center" style="color:#cc0000;" colspan="3">Não foram encontrados Registros.</td></tr>';
	}
	echo $html;
}

function validaSessao( $session, $modulo ){
	if( empty($session) ){
		echo "<script>
				alert('Falta dados na sessão!');
				window.location.href='siscap.php?modulo=$modulo&acao=A';
			  </script>";
		die;
	}
}

function insereTurma( $post = array(), $boAjax = false ){

//    ver($_REQUEST, d);

	global $db;
	extract( $post );

	$turdtini = ( !empty($turdtini) ? formata_data_sql($turdtini) : 'null');
	$turdtfim = ( !empty($turdtfim) ? formata_data_sql($turdtfim) : 'null');

	$turhrinipm = ( !empty($turhrinipm) ? $turhrinipm : '' );
	$turhrfimpm = ( !empty($turhrfimpm) ? $turhrfimpm : '' );

	if( empty($turid) ){
		$turturma = $db->pegaUm( "SELECT max(turturma) + 1 FROM siscap.turma WHERE curid = $curid" );
		$turturma = ( empty($turturma) ? '1' : $turturma );

		$sql = "INSERT INTO siscap.turma(curid, turdtinc, turdtini, turdtfim, turhrini, turhrfim,
	  									turperiodo, turcarga, turpublicoalvo, turinfo, turstatus, turturma, turmodalidade, turobs, turhrinipm, turhrfimpm)
				VALUES ($curid, now(), '$turdtini', '$turdtfim', '$turhrini', '$turhrfim',
	  									'$turperiodo', '$turcarga', '$turpublicoalvo', '$turinfo', '$turstatus', '$turturma', '$turmodalidade', '$turobs', '$turhrinipm', '$turhrfimpm') RETURNING turid";

		$turid = $db->pegaUm( $sql );

		if( !empty($facid) ){
			$sql = "DELETE FROM siscap.turma_facilitador WHERE turid = $turid";
			$db->executar( $sql );

			foreach($facid as $value){
    			$sql = "INSERT INTO siscap.turma_facilitador(turid, facid)
    					VALUES ($turid, $value)";
    			$db->executar( $sql );
			}
		}
	} else {
		$curidF = (!empty($curid) ? "curid = '$curid'," : '');
		$sql = "UPDATE siscap.turma SET
				  	$curidF
				  	turdtini = '$turdtini',
				  	turdtfim = '$turdtfim',
				  	turhrini = '$turhrini',
				  	turhrfim = '$turhrfim',
				  	turperiodo = '$turperiodo',
				  	turcarga = '$turcarga',
				  	turpublicoalvo = '$turpublicoalvo',
				  	turinfo = '$turinfo',
				  	turstatus = '$turstatus',
				  	turmodalidade = '$turmodalidade',
				  	turobs = '$turobs',
				  	turhrinipm = '$turhrinipm',
				  	turhrfimpm = '$turhrfimpm'
				WHERE
  					turid = $turid";

		$db->executar( $sql );

		if( !empty($facid) ){
			$sql = "DELETE FROM siscap.turma_facilitador WHERE turid = $turid";
			$db->executar( $sql );

			foreach($facid as $value){
    			$sql = "INSERT INTO siscap.turma_facilitador(turid, facid)
    					VALUES ($turid, $value)";
    			$db->executar( $sql );
			}
		}
	}
//	if( $boAjax ){
//		if( $db->commit() ){
//			echo $turid;
//		} else {
//			echo 'erro';
//		}
//	} else {
		if($db->commit()){
			$db->sucesso( 'principal/listaTurma' );
		} else {
			echo "<script>
					alert('Falha na Operação');
					window.location.href = 'siscap.php?modulo=principal/gerenciaTurma&acao=A';
				 </script>";
			exit();
		}
//	}
}

function montaListaFacilitadorTurma( $post = array() ){
	global $db;
	$filtro = "";
	$habilita = $post['habilita'];
	$habil = $habilita == 'N'  ? 'disabled="disabled"' : '';

	if( !empty( $post['turid'] ) ){
		$filtro = "WHERE facid not in (SELECT facid FROM siscap.turma_facilitador WHERE turid = ".$post['turid'].")";
	}
	$sql = "SELECT facid as codigo, facnome as descricao
			FROM siscap.facilitador
			$filtro
			ORDER BY facnome";
	$db->monta_combo("facid", $sql, $habilita, 'Selecione...', '', '', '', '265', 'S', 'facid', '', '', 'Facilitador' );
	$html.= '<input type="button" id="btIncluir" value="Incluir" '.$habil.' onclick="salvarTurma(\'ajax\');" />';

	if( $habilita == 'N' )
		$acoes = "'<center><img src=\"/imagens/excluir_01.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";
	else
		$acoes = "'<center><img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirFacilitadorTurma('||fa.facid||', '||tf.turid||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";

	if( !empty($post['turid']) ){
		$sql = "SELECT $acoes as acao, fa.facid, fa.faccpf, fa.facnome
				FROM siscap.facilitador fa
					inner join siscap.turma_facilitador tf on tf.facid = fa.facid
				WHERE
					tf.turid = ".$post['turid'];

		$arFacilitador = $db->carregar( $sql );
	}
	$arFacilitador = $arFacilitador ? $arFacilitador : array();

	$html.= '<br><table width="50%" id="tb_tabela" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Ações</strong></td>
						<td align="Center" class="title" width="30%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Facilitador</strong></td>
					</tr>
				</thead>';

	if( !empty($arFacilitador) ){
		foreach ($arFacilitador as $key => $v) {
			key % 2 ? $cor = "#f7f7f7" : $cor = "";

			$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td>'.$v['acao'].'</td>
					<td>'.$v['facnome'].'</td>
				</tr>';
		}
	} else {
		$html.= '<tr><td align="center" style="color:#cc0000;" colspan="3">Não foram encontrados Registros.</td></tr>';
	}
	$html.= '</table>';
	echo $html;
}

function carregaCursoTurmas( $curid ){
	global $db;

	$html .= '
				<td width="15%" class="subtitulodireita">Turmas:</td>
				<td width="85%">';

		$sql = "SELECT
					(case when (select count(atuid) from siscap.aluno_turma where turid = t.turid and alucpf = '{$_SESSION['usucpf']}' and atusituacao = 'A') = 0 and turstatus = 'IA' then
					'<center><a href=\"siscap.php?modulo=principal/informacaoTurma&acao=A&turid='|| turid ||'&curid='|| curid ||'\"><img src=\"/imagens/alterar.gif\" border=0 alt=\"Ir\" title=\"Efetuar Inscrição\"> </a></center>'
					else
					'<center><img src=\"/imagens/alterar_01.gif\" border=0 alt=\"Ir\" title=\"Inscrição já Efetuada\">
					 <img style=\"cursor:pointer\" src=\"/imagens/consultar.gif\" border=0 alt=\"Ir\" title=\"Visualizar ementa\" onclick=\"imprimir(' || turid || ')\"></center>'
					end) as acoes,
					to_char(turdtini, 'DD/MM/YYYY')||' a '||to_char(turdtfim, 'DD/MM/YYYY') as periodo,
					turhrinipm,
					turhrfimpm,
					turperiodo as tipoperiodo,
					turcarga,
					turhrini,
					turhrfim,
				    case when turperiodo = 'MA' then 'Matutino'
				         when turperiodo = 'VE' then 'Vespertino'
				         when turperiodo = 'IN' then 'Integral'
				         when turperiodo = 'NO' then 'Noturno'
				    end as turperiodo,
				    case when turstatus = 'EA' then 'Em andamento'
				        when turstatus = 'IA' then 'Inscrições Abertas'
				        when turstatus = 'IE' then 'Inscrições Encerradas'
				        when turstatus = 'CO' then 'Concluído'
				        when turstatus = 'AD' then 'Adiado'
				        when turstatus = 'CA' then 'Cancelado' end as turstatus, turturma,
				        turhrinipm||' às '||turhrfimpm as periodopm
				FROM
				  siscap.turma t
				WHERE curid = ".$curid;

		$arTurma = $db->carregar( $sql );
		$arTurma = $arTurma ? $arTurma : array();

		$html.= '<table id="tblform" class="listagem" width="60%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="left">
				<thead>
					<tr>';
//				if( $_SESSION['sisbaselogin'] ){
					$html.=	'<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Ações</strong></td>';
//				}
					$html.=	'<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Turmas</strong></td>
						<td align="Center" class="title" width="90%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Dados da Turma</strong></td>
					</tr>
				</thead>';
		if( $arTurma ){
			foreach ($arTurma as $v) {
				$arHoraIni = explode( ':', $v['turhrini'] );
				$arHoraFim = explode( ':', $v['turhrfim'] );
				$horaini = $arHoraIni[0].'h'.$arHoraIni[1];
				$horafim = $arHoraFim[0].'h'.$arHoraFim[1];

				$arHoraInipm = explode( ':', $v['turhrinipm'] );
				$arHoraFimpm = explode( ':', $v['turhrfim'] );
				$horainipm = $arHoraInipm[0].'h'.$arHoraInipm[1];
				$horafimpm = $arHoraFimpm[0].'h'.$arHoraFimpm[1];

				$html.= '<tr>';
//					if( $_SESSION['sisbaselogin'] ){
						$html.=	'<td rowspan="4">'.$v['acoes'].'</td>';
//					}
					if( $v['tipoperiodo'] == 'IN' && !empty($v['periodopm']) ){
						$cargahoraria = $horaini.' a '.$horafim.' Intervalo '.$horainipm.' a '.$horafimpm;
					} else {
						$cargahoraria = $horaini.' a '.$horafim;
					}
						$html.=	'<td rowspan="4">'.$v['turturma'].'</td>
							<td><b>Periodo:</b> '.$v['periodo'].'</td>
						</tr><tr>
							<td><b>Carga Horária:</b> '.$v['turcarga'].'h/a</td>
						</tr><tr>
							<td><b>Horário:</b> '.$cargahoraria.'</td>
						</tr><tr>
							<td><b>Status:</b> '.$v['turstatus'].'</td>
						</tr>';
			}
		} else {
			$html.= '<tr><td align="center" style="color:#cc0000;" colspan="3">Não foram encontrados Registros.</td></tr>';
		}

		$html.= '</table></td>';

	echo $html;
}

function carregaServidorAjax( $cpf ){
	global $db;
	$cpf = str_replace( array('.','-'), '', $cpf );

	$sql = "SELECT alu.alucpf, alu.alunome, alu.aluareaatuacao, alu.aluendtrabalho, alu.alumatchefia,
	  			alu.aluemail, alu.aluemail as aluemailconf, alu.aluteletrabalho, alu.aluteleresidencial, alu.alutelecelular, alu.alugecoodequip, alu.alugecoodproj,
	  			alu.alunivel, alu.alucurso, alu.aluarea, alu.alunomechefia, alu.aluemailchefia, alu.aluemailchefia as aluemailchefiaconf, alu.aluteletrabalhochefia, alu.alucargofuncaochefia,
	  			alu.aluneceateespecial, alu.aluatendespecial, alu.alumatsiape, alu.aluvinculomec, alu.aluorgao, alu.alulotacao, alu.alucargo, alu.alufuncao,
	            ac.alcconhecimento, ac.alcexperiencia, ac.alcexpectativas, to_char(usu.usudatanascimento, 'DD/MM/YYYY') as usudatanascimento,
	            case when usu.ususexo = 'M' then 'Masculino' else 'Feminino' end ususexo, usu.usucpf,
	            cs.nu_matricula_siape, cs.co_uorg_lotacao_servidor, cs.no_servidor,
				cs.co_funcao, cs.co_nivel_funcao, cs.ds_funcao, cs.ds_situacao_servidor,
				cs.co_orgao_lotacao_servidor, cs.sg_unidade_organizacional,
				cs.ds_orgao, cs.ds_cargo_emprego, cs.nu_cpf
			FROM siscap.aluno alu
				left join siscap.tb_cadastro_servidor cs on cs.nu_cpf = alu.alucpf
				left join seguranca.usuario usu on usu.usucpf = alu.alucpf
			    left join siscap.aluno_curso ac on ac.alucpf = alu.alucpf and ac.curid = {$_SESSION['siscap']['curid']}
			WHERE
				alu.alucpf = '".$cpf."'";

	$arDados = $db->pegaLinha( $sql );

	if( empty($arDados) ){
		$sql = "SELECT nu_matricula_siape, co_uorg_lotacao_servidor, no_servidor, co_funcao, co_nivel_funcao, ds_funcao, ds_situacao_servidor,
				  co_orgao_lotacao_servidor, sg_unidade_organizacional, ds_orgao, ds_cargo_emprego, nu_cpf
				FROM siscap.tb_cadastro_servidor WHERE nu_cpf = '".$cpf."'";
		$arDados = $db->pegaLinha( $sql );
	}

	$arDados['nu_matricula_siape'] = trim($arDados['nu_matricula_siape']);
	if( empty($arDados['nu_matricula_siape']) && $_SESSION['siscap']['tipo'] == 'relatorio' ){
		$arDados['nu_matricula_siape'] = campo_texto('alumatsiape', 'N', 'S', 'Matr&iacute;cula SIAPE', 7, 15, '[#]', '', '', '', 0, 'id=alumatsiape', '', $arDados['alumatsiape'] );
	} else {
		$arDados['nu_matricula_siape'] = (empty($arDados['nu_matricula_siape']) ? $arDados['alumatsiape'] : $arDados['nu_matricula_siape'] );
	}
	if( empty($arDados['ds_situacao_servidor']) ) $arDados['ds_situacao_servidor'] = campo_texto('aluvinculomec', 'N', 'S', 'V&iacute;nculo com o MEC', 30, 20, '', '', '', '', 0, 'id=aluvinculomec', '', $arDados['aluvinculomec'] );
	if( empty($arDados['ds_orgao']) ) {
		$arDados['ds_orgao'] = '<input type="hidden" name="bounidade" id="bounidade" value="S">'.campo_texto('aluorgao', 'N', 'S', 'Org&atilde;o', 60, 40, '', '', '', '', 0, 'id=aluorgao', '', $arDados['aluorgao'] ).'<img border="0" title="Indica campo obrigat&oacute;rio." src="../imagens/obrig.gif">';
	} else {
		$arDados['ds_orgao'] .= '<input type="hidden" name="bounidade" id="bounidade" value="N">';
	}
	if( empty($arDados['sg_unidade_organizacional']) ){
		$arDados['sg_unidade_organizacional'] = '<input type="hidden" name="bolocacao" id="bolocacao" value="S">'.campo_texto('alulotacao', 'N', 'S', 'Lota&ccedil;&atilde;o', 60, 40, '', '', '', '', 0, 'id=alulotacao', '', $arDados['alulotacao'] ).'<img border="0" title="Indica campo obrigat&oacute;rio." src="../imagens/obrig.gif">';
	} else {
		$arDados['sg_unidade_organizacional'] .= '<input type="hidden" name="bolocacao" id="bolocacao" value="N">';
	}
	if( empty($arDados['ds_cargo_emprego']) ){
		$arDados['ds_cargo_emprego'] = '<input type="hidden" name="bocargo" id="bocargo" value="S">'.campo_texto('alucargo', 'N', 'S', 'Cargo', 60, 40, '', '', '', '', 0, 'id=alucargo', '', $arDados['alucargo'] ).'<img border="0" title="Indica campo obrigat&oacute;rio." src="../imagens/obrig.gif">';
	} else {
		$arDados['ds_cargo_emprego'] .= '<input type="hidden" name="bocargo" id="bocargo" value="N">';
	}
	if( (empty($arDados['ds_funcao']) && empty($arDados['co_nivel_funcao'])) ){
		$arDados['ds_funcao'] = campo_texto('alufuncao', 'N', 'S', 'Fun&ccedil;&atilde;o', 60, 40, '', '', '', '', 0, 'id=alufuncao', '', $arDados['alufuncao'] );
	} else {
		$arDados['ds_funcao'] = $arDados['ds_funcao'] . ' ' . $arDados['co_nivel_funcao'];
	}

	$sql = "select count(atuid) from siscap.aluno_turma where turid = ".$_SESSION['siscap']['turid']." and alucpf = '{$cpf}' and atusituacao = 'A'";
	$boExiste = $db->pegaUm( $sql );
	if( $boExiste == 0 ) $arDados['inscrito'] = 'N';
	else $arDados['inscrito'] = 'S';

	echo simec_json_encode($arDados);
}

function montaCabecalhoFacilitador($facid)
{
    global $db;

    if($facid){

        $sql = "SELECT
                    faccpf,
                    facnome
                FROM siscap.facilitador
                WHERE facid = '$facid'";

        $rsFacilitador = $db->pegaLinha( $sql );

        if(count($rsFacilitador)){

            echo '<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
                    <tr>
                        <td class="SubtituloDireita" style="width: 15%">Facilitador:</td>
                        <td>'.$rsFacilitador['facnome'].'</td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">CPF:</td>
                        <td>'.$rsFacilitador['faccpf'].'</td>
                    </tr>
                  </table>';
        }
    }
}
?>