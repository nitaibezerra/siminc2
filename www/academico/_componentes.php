<?php

/**
 * Classe que possui os principais atributos do módulo
 *
 */
class academico{

	public $db;

	function __construct(){
		global $db;
		$this->db  = $db;
	}

	/**
	 * Função que trata os dados antes de utilizá-lo no banco
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 12/03/2009
	 * @param array $dados
	 */
	function quote( $dados ){

		foreach( $dados as $campo=>$valor ){
			if( !is_array( $dados[$campo] ) ){
				if( $valor == "" ){
					$dados[$campo] = 'NULL';
				} else {
					$dados[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
				}
			}
		}
		return $dados;
	}

	/**
	 * Função que formata os valores reais para inserí-los no banco
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 12/03/2009
	 * @param string $vlr
	 * @return string
	 */
	function formatanumero( $vlr ){
		$string = str_replace( ".", "", $vlr );
		$string = str_replace(",", ".", $string );

		return $string;
	}

	/**
	 * Função que cria o cabeçalho padrão com os dados da portaria
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 12/03/2009
	 * @param integer $prtid
	 * @return mixed
	 */
	function cabecalho( $prtid, $entid ){

		$sql = "SELECT
					p.prtnumero,
					p.prtidautprov,
					p.prtid as numcontrole,
					to_char(p.prtdtinclusao, 'DD/MM/YYY') as dtcriacao
				FROM
					academico.portarias p
				WHERE
					p.prtid = {$prtid}";

		$dados = $this->db->carregar( $sql );

		$sql = "";
		$sql_unidade = "SELECT
					e.entnome as campus,
					e2.entnome as unidade
				FROM entidade.entidade e
				INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid
				INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
				INNER JOIN entidade.entidade e2 ON fea.entid = e2.entid
				WHERE
					e.entid = {$entid}";

		if ($_SESSION["academico"]["tprnivel"]==1)
			$tipo_portaria='Autorização';
		else{

			$tipo_portaria='Provimento';

			if ( $dados[0]['prtidautprov'] ){

				$sql = "SELECT
						p.prtnumero,
						p.prtid as numcontrole,
						to_char(p.prtdtinclusao, 'DD/MM/YYYY') as dtcriacao
					FROM
						academico.portarias p
					WHERE
						p.prtid = {$dados[0]['prtidautprov']}";

				$dados_aut = $this->db->carregar( $sql );

			}


			$portaria_autorizacao=
				    '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Número de Controle da Autorização</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados_aut[0]['numcontrole'] . '</td>'
				   . '	</tr>'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Número da Portaria de Autorização</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados_aut[0]['prtnumero'] . '</td>'
				   . '	</tr>';
		}

		$dados1 = $this->db->carregar( $sql_unidade );
		$cabecalho = '<table class="tabela" align="center">'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Número da Portaria de '.$tipo_portaria.'</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados[0]['prtnumero'] . '</td>'
				   . '	</tr>'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Número de Controle</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados[0]['numcontrole'] . '</td>'
				   . '	</tr>'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Data de Criação</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados[0]['dtcriacao'] . '</td>'
				   . '	</tr>'
				   . $portaria_autorizacao
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Unidade</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados1[0]["unidade"] . '</td>'
				   . '	</tr>'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Campus</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados1[0]["campus"] . '</td>'
				   . '	</tr>'
				   . '</table>';

		return $cabecalho;
	}

	/**
	 * Enter description here...
	 *
	 * @param unknown_type $prtid
	 * @return unknown
	 */
	function cabecalho_ini( $prtid ){

		$sql = "SELECT
					p.prtnumero,
					p.prtidautprov,
					p.prtid as numcontrole,
					to_char(p.prtdtinclusao, 'DD/MM/YYYY') as dtcriacao
				FROM
					academico.portarias p
				WHERE
					p.prtid = ".$prtid;

		$dados = $this->db->carregar( $sql );

		if ( $_SESSION["academico"]["tprnivel"] == ACA_TPORTARIA_CONCURSO )
			$tipo_portaria='Autorização';
		else{

			$tipo_portaria='Provimento';
			$prtid = $dados[0]['prtidautprov'];

			$sql = "SELECT
					p.prtnumero,
					p.prtid as numcontrole,
					to_char(p.prtdtinclusao, 'DD/MM/YYYY') as dtcriacao
				FROM
					academico.portarias p
				WHERE
					p.prtid = ".$prtid;

			if ( !empty($dados[0]['prtidautprov']) ) {
				$dados_aut = $this->db->carregar( $sql );
			}

			$portaria_autorizacao=
				    '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Número de Controle da Autorização</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados_aut[0]['numcontrole'] . '</td>'
				   . '	</tr>'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Número da Portaria de Autorização</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados_aut[0]['prtnumero'] . '</td>'
				   . '	</tr>';
		}

		$cabecalho = '<table class="tabela" align="center">'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Número da Portaria de '.$tipo_portaria.'</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados[0]['prtnumero'] . '</td>'
				   . '	</tr>'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Número de Controle</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados[0]['numcontrole'] . '</td>'
				   . '	</tr>'
				    . $portaria_autorizacao
				   . '</table>';

		return $cabecalho;
	}

	/**
	 * Função que cria o cabeçalho padrão com os dados da portaria e do edital
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 18/03/2009
	 * @param integer $epdid
	 * @param integer $prtid
	 * @param integer $entid
	 * @return mixed
	 */
	function cabecalhoedital_ini( $prtid, $entid ){

		$sql = "SELECT
					e.entnome as campus,
					e2.entnome as unidade
				FROM
					entidade.entidade e
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				INNER JOIN
					entidade.funentassoc ea ON ea.fueid = ef.fueid
				INNER JOIN
					entidade.entidade e2 ON ea.entid = e2.entid
				WHERE
					e.entid = {$entid}";
		$dados = $this->db->carregar( $sql );

		$sql = "SELECT
					p.prtnumero,
					p.prtid as numcontrole,
					to_char(p.prtdtinclusao, 'DD/MM/YYYY') as dtcriacao,
					prtano
				FROM
					academico.portarias p
				WHERE
					p.prtid = {$prtid}";

		$dados_aut = $this->db->carregar( $sql );

		$portaria_autorizacao=
				    '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Nº de Controle da Portaria de Autorização</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados_aut[0]['numcontrole'] . '</td>'
				   . '	</tr>'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Número da Portaria de Autorização</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados_aut[0]['prtnumero'] . '</td>'
				   . '	</tr>'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Exercício</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados_aut[0]['prtano'] . '</td>'
				   . '	</tr>';

		$cabecalho = '<table class="tabela" align="center">'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Unidade</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados[0]["unidade"] . '</td>'
				   . '	</tr>'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Campus</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados[0]["campus"] . '</td>'
				   . '	</tr>'
				   . $portaria_autorizacao
				   . '	</table>';

		return $cabecalho;

	}

	/* Função que cria o cabeçalho padrão com os dados da portaria e do edital
	 * @param integer $entid
	 * @return mixed
	 */
	function cabecalho_minimo( $entid ){

		$sql = "SELECT
					e.entnome as campus,
					e2.entnome as unidade
				FROM
					entidade.entidade e
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				INNER JOIN
					entidade.funentassoc ea ON ea.fueid = ef.fueid
				INNER JOIN
					entidade.entidade e2 ON ea.entid = e2.entid
				WHERE
					e.entid = {$entid}";
		$dados = $this->db->carregar( $sql );

		$cabecalho = '<table class="tabela" align="center">'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Unidade</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados[0]["unidade"] . '</td>'
				   . '	</tr>'
				   . '	<tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Campus</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . $dados[0]["campus"] . '</td>'
				   . '	</tr>'
				   . '	</table>';

		return $cabecalho;

	}

	/* Função que cria o cabeçalho padrão para entidade
	 * @param integer $entid
	 * @return mixed
	 */
	function cabecalho_entidade( $entid, $obrid = '', $curid = '' ){

		$orgid = $_SESSION['academico']['orgid'];
		if($orgid == ACA_ORGAO_SUPERIOR){
			$orgao = "Educação Superior";
		}else{
			$orgao = "Educação Profissional";
		}

		$entidentidade = $this->buscaentidade($entid);

		//se for um campus
		if($entidentidade) {
			$sql = "SELECT ent.entnome as campus, ende.estuf, mundescricao, uo.entnome AS unidadeorc, uo.entid as unidadeorcid
					FROM entidade.entidade ent
					INNER JOIN entidade.funcaoentidade ef ON ent.entid = ef.entid
					INNER JOIN entidade.funentassoc ea ON ef.fueid = ea.fueid
					INNER JOIN entidade.entidade uo ON uo.entid = ea.entid
					LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
					LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
					WHERE ent.entid = '". $entid ."' ORDER BY ent.entnome";

		} else {

			$sql = "SELECT ent.entid as unidadeorcid, ent.entnome as unidadeorc, ende.estuf, mundescricao
					FROM entidade.entidade ent
					--INNER JOIN entidade.funcaoentidade ef ON ef.entid = ent.entid
					LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
					LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
					WHERE ent.entid = '". $entid ."' ORDER BY ent.entnome";

		}
		$dados = $this->db->pegaLinha( $sql );

		if ( !empty($obrid) ){

			$sql = "SELECT obrdesc FROM obras.obrainfraestrutura WHERE obrid = {$obrid}";
			$nome_obra = $this->db->pegaUm( $sql );

			if ( !empty($nome_obra) ){
				$nome_obra = "<tr>"
						   . "	<td class='SubTituloDireita' width='250px;'>Nome da Obra:</td><td>".$nome_obra."</td>"
						   . "</tr>";
			}

		}

		if ( !empty($curid) ){

			$sql = "SELECT
						curdsc,
						CASE WHEN curinicioexec is not null THEN curinicioexec ELSE 'Não Informado' END as ano
					FROM
						academico.curso WHERE curid = {$curid}";

			$curso = $this->db->pegaLinha( $sql );

			if ( !empty($curso) ){
				$dados_curso = "<tr><td class='SubTituloDireita' width='250px;'>Curso:</td><td>".$curso['curdsc']."</td></tr>"
							 . "<tr><td class='SubTituloDireita' width='250px;'>Ano Base:</td><td>".$curso['ano']."</td></tr>";
			}
		}

        if(!$entidentidade && $orgid==ACA_ORGAO_TECNICO && ($this->db->testa_superuser() || academico_possui_perfil(array(PERFIL_REITOR, PERFIL_DIRETOR_CAMPUS, PERFIL_INTERLOCUTOR_INSTITUTO, PERFIL_INTERLOCUTOR_CAMPUS, PERFIL_PROREITOR)))){
            $cockpit = "<tr><td class='SubTituloDireita' width='250px;'>Painel de Monitoramento:</td><td><a href='/pde/estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid=".$entid."&redefederal=SIM'><font size=+1>Dados do Instituto</font></a></td></tr>";
        }

		$cabecalho = '';
		if($dados) {
			$cabecalho = "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>"
			. "<tr>"
			. "<td class='SubTituloDireita' width='250px;'>Tipo Ensino:</td><td>".$orgao."</td>"
			. "</tr>"
			. "<tr>"
			. "<td class='SubTituloDireita'>Instituição:</td><td>".$dados['unidadeorc']."</td>"
			. "</tr>";
			if($entidentidade) {
				$cabecalho .= "<tr>"
				. "<td class='SubTituloDireita'>Campus / Uned:</td><td>".$dados['campus']."</td>"
				. "</tr>";
			}
			$cabecalho .= "<tr>"
			. "<td class='SubTituloDireita'>UF / Munícipio:</td><td>".$dados['estuf']." / ".$dados['mundescricao']."</td>"
			. "</tr>"
            . $cockpit
			. $nome_obra
			. $dados_curso
			. "</table>";
		} else {
			$cabecalho .=("<script>
					alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
					window.location='?modulo=inicio&acao=C';
				 </script>");
		}

		return $cabecalho;
	}

	/**
	 * Função que cria o cabeçalho padrão com os dados da portaria e do edital
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 18/03/2009
	 * @param integer $epdid
	 * @param integer $prtid
	 * @param integer $entid
	 * @return mixed
	 */
	function cabecalhoedital( $epdid, $prtid, $entid ){
		$sql = "SELECT
					edpnumero as numero,
					to_char(edpdtinclusao, 'DD/MM/YYYY') as data
				FROM
					academico.editalportaria
				WHERE
					edpid = {$epdid}";

		$dados = $this->db->carregar( $sql );

		$cabecalho  = $this->cabecalhoedital_ini( $prtid, $entid );
		$cabecalho .= '<table class="tabela" align="center"><tr>'
				   . '		<td class="SubTituloEsquerda" style="text-align:right;" >Número do Edital</td>'
				   . '		<td width="80%" class="SubTituloDireita" style="text-align:left;background:#EEE;"> ' . ($dados[0]['numero'] ? $dados[0]['numero'] : ' Não Informado') . '</td>'
				   . '	</tr></table>';

		return $cabecalho;

	}

	/**
	 * Função que lista todas as unidades cadastras em banco para poder inserir
	 * nas portarias
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 12/03/2009
	 * @param integer $orgid
	 * @return mixed
	 */
	function listaunidadeselecao( $orgid ){

		$coluna = "";

		switch ( $orgid ){
			case '1':
				$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
			break;
			case '2':
				$funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
			break;
			case '3':
				$funid = " in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";
			break;
		}

		// Busca as unidades inseridas na portaria
		$sql = "SELECT
					entid as codigo
				FROM
					academico.entidadeportaria
				WHERE
					prtid = {$_SESSION["academico"]['prtid']}";

		$inseridas = $this->db->carregar( $sql );

		// Busca as unidades
		$sql = "SELECT
					e.entid as codigo,
					e.entnome as descricao
				FROM
					entidade.entidade e
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				WHERE
					e.entstatus = 'A' AND ef.funid {$funid}
				ORDER BY
					e.entnome";

		$dados = $this->db->carregar( $sql );

		if ( is_array($dados) ){

			for( $i = 0; $i < count( $dados ); $i++ ){

				for( $k = 0; $k < count( $inseridas ); $k++ ){

					if($inseridas[$k]['codigo'] == $dados[$i]['codigo'] ) {
						$checked = 'checked="checked"';
						break;
					} else {
						$checked = '';
					}

				}

				$cor = ($i % 2) ? '#f4f4f4' : '#ffffff';

				$coluna .= "<tr bgColor=\"" . $cor . "\">"
						 . "	<td align=\"center\">"
						 . "		<input type=\"checkbox\" id=\"entid\" name=\"entid[]\" value=\"" . $dados[$i]['codigo'] . "\" " . $checked . ">"
						 . "	</td>"
						 . "	<td>" . $dados[$i]['descricao'] . "</td>"
						 . "</tr>";
			}

		}

		return $coluna;

	}

	/**
	 * Função que lista as todas as unidades de acordo com o orgão
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 12/03/2009
	 * @param integer $orgid
	 */
	function listaunidades( $orgid ){

		switch ( $orgid ){
			case '1':
				$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
			break;
			case '2':
				$funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
			break;
			case '3':
				$funid = " in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";
			break;
		}

		$sql = "SELECT
					'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || e.entid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || e.entid || ' ); abreconteudo(\'academico.php?modulo=principal/planodistribuicaocargos&acao=C&subAcao=gravarCarga&carga=' || e.entid || '&params=\' + params, ' || e.entid || ');\"/></center>' as img,
					UPPER(entnome),
					'<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' || e.entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
				FROM
					academico.entidadeportaria ae
				INNER JOIN
					entidade.entidade e ON ae.entid = e.entid
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				WHERE
					e.entstatus = 'A' AND ef.funid {$funid}
					AND ae.prtid = {$_SESSION["academico"]["prtid"]}
				ORDER BY
					e.entnome";

		$cabecalho = array('Ação', 'Unidade');
		$this->db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '');

	}

	/**
	 * Função que lista as todas as unidades que possuem portarias autorizadas
	 *
	 * @author Werter Dais Almeida
	 * @since 02/04/2009
	 * @param integer $orgid
	 */
	function listaunidadesedital( $orgid, $unidades = null ){

		switch ( $orgid ){
			case '1':
				$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
			break;
			case '2':
				$funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "', '" . ACA_ID_ESCOLAS_AGROTECNICAS . "')";
			break;
			case '3':
				$funid = " in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";
			break;
			default:
				$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
			break;
		}

		$sql = "SELECT
					'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || e.entid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || e.entid || ' ); abreconteudo(\'academico.php?modulo=principal/listaUnidade&acao=A&carga=' || e.entid || '&params=\' + params, ' || e.entid || ');\"/></center>' as img,
					'<a href=\"?modulo=principal/dadosentidade&acao=A&entidunidade=' || e.entid || '\">' || UPPER(entnome) || '</a>',
					'<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' || e.entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
				FROM
					academico.entidadeportaria ae
				INNER JOIN
					entidade.entidade e ON ae.entid = e.entid
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				WHERE
					e.entstatus = 'A' AND ef.funid {$funid}
					" . ( $unidades ? " AND e.entid in (" . implode(',', $unidades) . ")" : "") ."
				GROUP BY e.entid, e.entnome
				ORDER BY
					e.entnome";

		$cabecalho = array('Ação', 'Unidade');
		$this->db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '');

	}

	/* Função que lista todos os campus cadastrados no banco de acordo com o orgão
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 12/03/2009
	 * @param ingeter $orgid
	 */
	function listacampusedital( $orgid ){

		switch ( $orgid ){
			case '1':
				$funid = ACA_ID_CAMPUS;
			break;
			case '2':
				$funid = ACA_ID_UNED;
			break;
		}

		$sql = "SELECT
					'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'/academico/academico.php?modulo=principal/autorizacaodeconcursos&acao=C&entid=' || e.entid || '\'\">',
					'<a href=\"/academico/academico.php?modulo=principal/autorizacaodeconcursos&acao=C&entid=' || e.entid || '\">' || upper(e.entnome) || '</a>' as nome
				FROM entidade.entidade e2
				INNER JOIN entidade.funcaoentidade fen ON fen.entid = e2.entid
				INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
				INNER JOIN entidade.entidade e ON e2.entid = e.entid
				INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = e.entid
				INNER JOIN academico.entidadeportaria ep ON fea.entid = ep.entid
				WHERE
					e.entstatus = 'A' AND fen2.funid = {$funid} AND
					ep.prtid = {$_SESSION["academico"]["prtid"]}
				ORDER BY
					e.entnome";

		$cabecalho = array('Ação', 'Campus');
		$this->db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '');

	}

	/**
	 * Função que lista todos os campus cadastrados no banco de acordo com o orgão
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 12/03/2009
	 * @param ingeter $orgid
	 */
	function listacampus( $orgid ){

		switch ( $orgid ){
			case '1':
				$funid = ACA_ID_CAMPUS;
			break;
			case '2':
				$funid = ACA_ID_UNED;
			break;
		}

		$sql = "SELECT
					'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'/academico/academico.php?modulo=principal/autorizacaodeconcursos&acao=C&entid=' || e.entid || '\'\">',
					'<a href=\"/academico/academico.php?modulo=principal/autorizacaodeconcursos&acao=C&entid=' || e.entid || '\">' || upper(e.entnome) || '</a>' as nome
				FROM
					entidade.entidade e2
				INNER JOIN
					entidade.entidade e ON e2.entid = e.entid
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				INNER JOIN
					entidade.funentassoc ea ON ef.fueid = ea.fueid
				INNER JOIN
					academico.entidadeportaria ep ON ea.entid = ep.entid
				WHERE
					e.entstatus = 'A' AND ef.funid = {$funid} AND
					ep.prtid = {$_SESSION["academico"]["prtid"]}
				ORDER BY
					e.entnome";

		$cabecalho = array('Ação', 'Campus');
		$this->db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '');

	}

	/**
	 * Função que lista os campus de uma determinada entidade
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 12/03/2009
	 * @param integer $entid
	 */
	function listacampusentidade( $entid, $orgid ){

		switch ( $orgid ){
			case '1':
				$funid = ACA_ID_CAMPUS;
			break;
			case '2':
				$funid = ACA_ID_UNED;
			break;
		}

		$sql = "SELECT
					'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'/academico/academico.php?modulo=principal/autorizacaodeconcursos&acao=C&entid=' || e.entid || '\'\">',
					'<a href=\"/academico/academico.php?modulo=principal/autorizacaodeconcursos&acao=C&entid=' || e.entid || '\">' || upper(e.entnome) || '</a>' as nome
				FROM
					entidade.entidade e2
				INNER JOIN
					entidade.entidade e ON e2.entid = e.entid
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				INNER JOIN
					entidade.funentassoc ea ON ea.fueid = ef.fueid
				INNER JOIN
					academico.entidadeportaria ep ON ea.entid = ep.entid
				WHERE
					ea.entid = {$entid} AND
					e.entstatus = 'A' AND ef.funid = {$funid}
					AND ep.prtid = {$_SESSION["academico"]["prtid"]}
				ORDER BY
					e.entnome";

		$cabecalho = array('Ação', 'Campus');
		$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');

	}

	/* Função que lista as reitorias, complementar à função acima
		 * Autor: Afonso Alves Ribeiro
		 * Data: 03/05/2011
		 * Parametros: $entid: integer
		 * 			   $orgid: integer
		 */

		function listareitoriaentidade( $entid, $orgid ){

				if( $orgid ){
						$funid = ACA_ID_REITORIA;
				}else{
						$funid = ACA_ID_UNED;
				}

				$sql = "SELECT
						'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'/academico/academico.php?modulo=principal/autorizacaodeconcursos&acao=C&entid=' || e.entid || '\'\">',
						'<a href=\"/academico/academico.php?modulo=principal/autorizacaodeconcursos&acao=C&entid=' || e.entid || '\">' || upper(e.entnome) || '</a>' as nome
						FROM
							entidade.entidade e
						INNER JOIN
							entidade.funcaoentidade ef ON ef.entid = e.entid
						INNER JOIN
							entidade.funentassoc ea ON ea.fueid = ef.fueid
						WHERE
							ea.entid = {$entid} AND
							e.entstatus = 'A' AND ef.funid = {$funid}
						ORDER BY
							e.entnome";

				$cabecalho = array('Ação','Reitoria');
				$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');

			}

	/**
	 * Função que vincula as unidades às portarias
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 12/03/2009
	 * @param integer $entid
	 * @param integer $prtid
	 */
	function cadastraunidades( $entid, $prtid ){

		$sql = "DELETE FROM academico.entidadeportaria WHERE prtid = {$prtid}";
		$this->db->executar( $sql );

		if ( is_array( $entid ) ){

			for( $i = 0; $i < count( $entid ); $i++ ){

				$sql = "INSERT INTO
						academico.entidadeportaria (entid, prtid)
					VALUES
						( {$entid[$i]} , {$prtid})";

				$this->db->executar( $sql );

			}

		}

		$this->db->commit();

		echo '<script>
				alert("Operação realizada com sucesso!");
				window.parent.opener.location.href = window.opener.location;
				self.close();
			  </script>';

	}

	/**
	 * Bloqueia o sistema durante o período informado pelo usuário
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 25/05/2009
	 * @param array $dados
	 */
	function bloqueiasistema( $dados ){

		if ( !empty($dados) ){

			$dados["blsdtinicio"]  = formata_data_sql($dados["blsdtinicio"]);
			$dados["blsdttermino"] = formata_data_sql($dados["blsdttermino"]);

			if ( $dados['blsid'] ){

				$sql = "UPDATE
							academico.bloqueiosistema
						SET
							blsdtinicio  = '{$dados["blsdtinicio"]}',
							blsdttermino = '{$dados["blsdttermino"]}',
							blsmotivo    = '{$dados["blsmotivo"]}',
							orgid 		 = '{$dados["orgid"]}',
							usucpf 		 = '{$_SESSION["usucpf"]}'
						WHERE
							blsid =  {$dados['blsid']}";

				$this->db->executar($sql);

			}else{

				$sql = "SELECT blsid FROM academico.bloqueiosistema
						WHERE blsstatus = 'A' AND orgid={$dados["orgid"]}";

				$bloqueado = $this->db->pegaUm($sql);

				if ( $bloqueado ){
					print "<script>"
						 ."		alert('O sistema já se encontra bloqueado para este tipo de ensino!');"
						 ."		history.back(-1);"
						 ."</script>";
					die;
				}

				$sql = "INSERT INTO academico.bloqueiosistema
							(blsdtinicio, blsdttermino,
							 blsmotivo, blsstatus,
							 blsdtinclusao, orgid, usucpf)
						VALUES
							('{$dados["blsdtinicio"]}', '{$dados["blsdttermino"]}',
							 '{$dados["blsmotivo"]}', 'A',
							 'now()', '{$dados["orgid"]}',
							 '{$_SESSION["usucpf"]}')";

				$this->db->executar($sql);

			}

		}

		$this->db->commit();
		$this->db->sucesso('sistema/bloqueio/bloqueio_sistema', '');

	}

	/**
	 * Enter description here...
	 *
	 * @param unknown_type $blsid
	 * @return unknown
	 */
	function buscadadobloqueio( $blsid ){

		$sql = "SELECT
					blsid, orgid, blsdtinicio, blsdttermino, trim(blsmotivo) as blsmotivo
				FROM academico.bloqueiosistema
				WHERE blsid = {$blsid}";

		$dados = $this->db->pegaLinha($sql);

		return $dados;

	}

	/**
	 * Enter description here...
	 *
	 * @param unknown_type $blsid
	 */
	function desbloqueiasistema( $blsid ){

		if ( $blsid ){

			$sql = "UPDATE
						academico.bloqueiosistema
					SET
						blsstatus = 'I',
						blsdtdesbloqueio = 'now()',
						usucpfdesbloqueio = '{$_SESSION["usucpf"]}'
					WHERE
						blsid = {$blsid}";

			$this->db->executar($sql);
			$this->db->commit();
			$this->db->sucesso('sistema/bloqueio/bloqueio_sistema', '');

		}

	}

	function autorizacaoespecial( $dados ){

		if ( !empty($dados) ){

			$dados["autdtinicio"]  = formata_data_sql($dados["autdtinicio"]);
			$dados["autdttermino"] = formata_data_sql($dados["autdttermino"]);

			if (!$dados['autid']){

				$sql = "INSERT INTO academico.autorizacaoespecial (orgid,
																   autdtinicio,
																   autdttermino,
																   usucpf,
																   autstatus,
																   autdtinclusao)
						 VALUES
							({$dados["orgid"]},
							 '{$dados["autdtinicio"]}',
							 '{$dados["autdttermino"]}',
							 '{$_SESSION['usucpf']}',
							 'A',
							 'now()') returning autid";

				$autid = $this->db->pegaUm($sql);

				foreach( $dados['unidade'] as $chave=>$valor ){

					$sql = "INSERT INTO academico.autorizacaoentidade (autid, entid)
							VALUES ({$autid}, {$valor})";

					$this->db->executar($sql);

				}

			}else{

				$sql = "UPDATE
							academico.autorizacaoespecial
						SET
							orgid = {$dados["orgid"]},
						    autdtinicio = '{$dados["autdtinicio"]}',
						    autdttermino = '{$dados["autdttermino"]}',
						    usucpf = '{$_SESSION['usucpf']}'
						WHERE
							autid = {$dados['autid']}";

				$this->db->executar($sql);

				$sql = "DELETE FROM academico.autorizacaoentidade
						WHERE autid = {$dados['autid']}";

				$this->db->executar($sql);

				foreach( $dados['unidade'] as $chave=>$valor ){

					$sql = "INSERT INTO academico.autorizacaoentidade (autid, entid)
							VALUES ({$dados['autid']}, {$valor})";

					$this->db->executar($sql);

				}

			}

			$this->db->commit();
			$this->db->sucesso('sistema/bloqueio/autorizacaoespecial', '');

		}
	}

	function buscadadoautorizacao( $autid ){

		$sql = "SELECT autid, autdtinicio, autdttermino, orgid
				FROM academico.autorizacaoespecial
				WHERE autid = {$autid}";

		$dados = $this->db->pegaLinha($sql);

		$sql = "SELECT
					ae.entid as codigo,
					e.entnome as descricao
				FROM
					academico.autorizacaoentidade ae
				INNER JOIN
					entidade.entidade e ON e.entid = ae.entid
				WHERE
					autid = {$autid}";

		$dados['unidade'] = $this->db->carregar($sql);

		return $dados;

	}

	function excluirautorizacaoespecial( $autid ){

		$sql = "UPDATE
					academico.autorizacaoespecial
				SET
					autstatus = 'I'
				WHERE
					autid = {$autid}";

		$this->db->executar($sql);

		$sql = "DELETE FROM academico.autorizacaoentidade
						WHERE autid = {$autid}";

		$this->db->executar($sql);

		$this->db->commit();
		$this->db->sucesso('sistema/bloqueio/autorizacaoespecial', '');

	}

	/**
	 * Função que lista as unidades da tela iniciao de acordo com o orgid
	 * @author Fernando Araújo Bagno da Silva
	 * @since 07/07/2009
	 *
	 */
	function academico_listaunidades( $orgid ){

		$filtro = self::academico_retornarfiltropesquisa();

		switch ( $orgid ){

			case '1':
				$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
			break;

			case '2':
				$funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
			break;
			case '3':
				$funid = " in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";
			break;
		}

		if ( $this->db->testa_superuser() || academico_possui_perfil(PERFIL_ADMINISTRADOR) || academico_possui_perfil_sem_vinculo() || academico_possui_perfil(PERFIL_MECCADASTRO) || academico_possui_perfil_resp_tipo_ensino()){

			$sql = "SELECT
					'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || e.entid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || e.entid || ' ); formatarParametros();abreconteudo(\'academico.php?modulo=inicio&acao=C&subAcao=gravarCarga&orgid={$_REQUEST['orgid']}&carga=' || e.entid || '&params=\' + params, ' || e.entid || ');\"/></center>' as img,
					CASE WHEN entsig <> '' THEN
						'<a style=\"cursor:pointer;\" onclick=\"academico_abreSistema( ' || e.entid || ' );\">' || UPPER(entsig) ||  ' - ' || UPPER(entnome) || '</a>'
						ELSE
						'<a style=\"cursor:pointer;\" onclick=\"academico_abreSistema( ' || e.entid || ' );\">' || UPPER(entnome) ||  '</a>' END as nome,
					upper(mun.mundescricao) as municipio, upper(mun.estuf) as uf,
					'<tr>
                                            <td style=\"padding:0px;margin:0;\"></td>
                                            <td id=\"td' || e.entid || '\" colspan=\"4\" style=\"padding:0px;display:none;border: 5px red\"></td>
                                            <td style=\"padding:0px;margin:0;\"></td>
                                        </tr>' as tr
				FROM
					entidade.entidade e
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				LEFT JOIN
					entidade.endereco ed ON ed.entid = e.entid
				LEFT JOIN
					territorios.municipio mun ON mun.muncod = ed.muncod
				WHERE
					e.entstatus = 'A' AND ef.funid ". $funid . $filtro . "
				GROUP BY e.entid, e.entnome , e.entsig, mun.mundescricao, mun.estuf
				ORDER BY
					 e.entsig, e.entnome";

		}else{

			$sql = "SELECT
					'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || e.entid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || e.entid || ' ); formatarParametros();abreconteudo(\'academico.php?modulo=inicio&acao=C&subAcao=gravarCarga&orgid={$_REQUEST['orgid']}&carga=' || e.entid || '&params=\' + params, ' || e.entid || ');\"/></center>' as img,
					CASE WHEN entsig <> '' THEN
						'<a style=\"cursor:pointer;\" onclick=\"academico_abreSistema( ' || e.entid || ' );\">' || UPPER(entsig) ||  ' - ' || UPPER(entnome) || '</a>'
						ELSE
						'<a style=\"cursor:pointer;\" onclick=\"academico_abreSistema( ' || e.entid || ' );\">' || UPPER(entnome) ||  '</a>' END as nome,
					upper(mun.mundescricao) as municipio, upper(mun.estuf) as uf,
					'<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' || e.entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
				FROM
					entidade.entidade e
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				LEFT JOIN
					entidade.endereco ed ON ed.entid = e.entid
				LEFT JOIN
					territorios.municipio mun ON mun.muncod = ed.muncod
				INNER JOIN
					academico.usuarioresponsabilidade ur ON ur.entid = e.entid
															AND ur.rpustatus = 'A'
				WHERE
					e.entstatus = 'A' AND ef.funid ". $funid . $filtro . " AND ur.usucpf = '{$_SESSION['usucpf']}'
				GROUP BY e.entid, e.entnome,  e.entsig, mun.mundescricao, mun.estuf
				ORDER BY
					 e.entsig, e.entnome";

		}

		$cabecalho = array('Ação', 'Instituição', 'Município', 'UF');
		//dbg(simec_htmlentities($sql),1);
		$this->db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '');


	}

	/**
	 * Função cria os filtros da pesquisa da tela inicial do módulo
	 * @author Fernando Araújo Bagno da Silva
	 * @since 07/07/2009
	 *
	 */
	function academico_retornarfiltropesquisa(){

		$filtro .= !empty( $_REQUEST["entid"] ) ? ' AND e.entid'  . " = {$_REQUEST["entid"]}"    : '';
		$filtro .= !empty( $_REQUEST["estuf"] ) ? ' AND ed.estuf' . " = '{$_REQUEST["estuf"]}'"  : '';

		return $filtro;

	}

    function academico_listacampus( $entid, $orgid ){
        global $db;

        $perfil = pegaPerfilGeral();

        switch ( $orgid ){
            case '1':
                $funid = ACA_ID_CAMPUS;
            break;
            case '2':
                $funid = ACA_ID_UNED;
            break;
        }

        $arrCpf = array( "cpf1" => '', "cpf2" => '', "cpf3" =>'', "cpf4" =>'' );

        if( in_array( $_SESSION['usucpf'] , $arrCpf ) ){
            $botaoExcliur="'     <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"inativarcampus(' || e.entid || ');\">',";
        }else{
            $botaoExcliur= ',';
        }

        if($funid){
            #PERFIL DIRETOR DO CAMPUS SO PODERAR VISUALIZAR OS CAMPUS ATRIBUIDOS A ELE.
            if( in_array(PERFIL_DIRETOR_CAMPUS, $perfil) ){
                $sql = "
                    SELECT  DISTINCT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"abredadoscampus(' || e.entid || ');\">'
                            {$botaoExcliur}
                            '<a style=\"cursor:pointer;\" onclick=\"abredadoscampus(' || e.entid || ');\">' || upper(e.entnome) || '</a>' as nome,
                            upper(mun.mundescricao) as municipio,
                            upper(mun.estuf) as uf

                    FROM entidade.entidade e

                    INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid AND fe.funid in (17,18)
                    INNER JOIN academico.usuarioresponsabilidade ur ON ur.entid = e.entid AND ur.rpustatus = 'A'

                    LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
                    LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod

                    WHERE ur.rpustatus='A' AND ur.usucpf = '{$_SESSION['usucpf']}' AND ur.pflcod = ".PERFIL_DIRETOR_CAMPUS."

                    ORDER BY nome
                ";
            }else{
                $sql = "
                    SELECT  '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"abredadoscampus(' || e.entid || ');\">'
                            $botaoExcliur
                            '<a style=\"cursor:pointer;\" onclick=\"abredadoscampus(' || e.entid || ');\">' || upper(e.entnome) || '</a>' as nome,
                            upper(mun.mundescricao) as municipio, upper(mun.estuf) as uf

                    FROM entidade.entidade e2

                    INNER JOIN entidade.entidade e ON e2.entid = e.entid
                    INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
                    INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
                    LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
                    LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod

                    WHERE ea.entid = {$entid} AND e.entstatus = 'A' AND ef.funid = {$funid}

                    ORDER BY e.entnome
                ";
            }
        } else {
            $sql = array();
        }
        $cabecalho = array('Ação', 'Campus', 'Município', 'UF');
        $alinhamento = Array('center', '', '', '');
        $tamanho = Array('5%', '', '', '5%');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
        //$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '95%');
        
    }

    function academico_listareitoria( $entid, $orgid ){
        global $db;
        
        if( $orgid ){
            $funid = ACA_ID_REITORIA;
        }else{
            $funid = ACA_ID_UNED;
        }

        $arrCpf = array( "cpf1" => '', "cpf2" => '', "cpf3" =>'', "cpf4" =>'' );
        if( in_array( $_SESSION['usucpf'] , $arrCpf ) ){
            $botaoExcliur="'     <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"inativarreitoria(' || e.entid || ');\">',";
        }else{
            $botaoExcliur= ',';
        }

        $sql = "
            SELECT  '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"abredadosreitoria(' || e.entid || ');\">'
                    $botaoExcliur
                    '<a style=\"cursor:pointer;\" onclick=\"abredadosreitoria(' || e.entid || ');\">' || upper(e.entnome) || '</a>' as nome
                    
            FROM entidade.entidade e2
            
            INNER JOIN entidade.entidade e ON e2.entid = e.entid
            INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
            INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
            
            WHERE ea.entid = {$entid} AND e.entstatus = 'A' AND ef.funid = {$funid}
                
            ORDER BY e.entnome
        ";

        $cabecalho = array('Ação', 'Reitoria');
        $alinhamento = Array('center', '');
        $tamanho = Array('5%', '');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
        //$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '95%');

}
    
	function listaCampusAssistenciaEstudantil( $entid, $orgid ){

		switch ( $orgid ){
			case '1':
				$funid = ACA_ID_CAMPUS;
			break;
			case '2':
				$funid = ACA_ID_UNED;
			break;
		}

		if($funid) {
			$sql = "SELECT
						'<a style=\"cursor:pointer;\" onclick=\"abreAssistenciaEstudantil(' || e.entid || ');\">' || upper(e.entnome) || '</a>' as nome,
						upper(mun.mundescricao) as municipio, upper(mun.estuf) as uf
					FROM
						entidade.entidade e2
					INNER JOIN
						entidade.entidade e ON e2.entid = e.entid
					INNER JOIN
						entidade.funcaoentidade ef ON ef.entid = e.entid
					INNER JOIN
						entidade.funentassoc ea ON ea.fueid = ef.fueid
					LEFT JOIN
						entidade.endereco ed ON ed.entid = e.entid
					LEFT JOIN
						territorios.municipio mun ON mun.muncod = ed.muncod
					WHERE
						ea.entid = {$entid} AND
						e.entstatus = 'A' AND ef.funid = {$funid}
					ORDER BY
						e.entnome";
		} else {
			$sql = array();
		}


		$cabecalho = array('Campus', 'Município', 'UF');
		$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '95%');

	}
}


/**
 * Classe que possui todos os atributos em relação às unidades,
 * valores projetados e acumulados do módulo
 *
 */
class autoriazacaoconcursos extends academico{

	public $lnpvalor = null;
	public $acpvalor = null;

	function __construct(){

		parent::__construct();
		$this->setlnpvalor($dados['lnpvalor']);
		$this->setacpvalor($dados['acpvalor']);

	}

	// Funções SET
	public function setlnpvalor( $vlr ) {
		$this->lnpvalor = $vlr;
	}
	public function setacpvalor( $vlr ) {
		$this->acpvalor = $vlr;
	}

	// Funções GET
	public function getlnpvalor() {
		return $this->lnpvalor;
	}
	public function getacpvalor() {
		return $this->acpvalor;
	}

	/**
	 * Função que busca o programa da portaria
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 03/04/2009
	 * @param integer $prtid
	 * @return integer
	 */
	function buscaprogramaportaria( $prtid ){

		global $db;

		$sql = "SELECT prgid FROM academico.portarias WHERE prtid = {$prtid}";

		return $this->db->pegaUm( $sql );

	}

	/**
	 * Função que busca os dados de autorizado
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 12/03/2009
	 * @param integer $prtid
	 * @return array
	 */
	function buscaautorizado( $prtid, $clsid, $entidcampus ){

		$entidade = $this->buscaentidade( $entidcampus );

		$sql = "SELECT
					coalesce(sum(l.lnpvalor),0) as autorizado
				FROM
					academico.lancamentosportaria l
				INNER JOIN academico.portarias p ON p.prtid = l.prtid
				WHERE
					l.prtid = {$prtid} AND
					l.clsid = {$clsid} AND
					l.entidentidade = {$entidade} AND
					l.entidcampus = {$entidcampus} ";
		$resultado = $this->db->pegaUm( $sql );
		return $resultado;

	}

	/**
	 * Enter description here...
	 *
	 * @param unknown_type $prtidautprov
	 * @param unknown_type $clsid
	 * @param unknown_type $entidcampus
	 * @return unknown
	 */
	function buscalancado( $prtidautprov, $clsid, $entidcampus ){

		$entidade = $this->buscaentidade( $entidcampus );

		$sql = "SELECT
				CASE WHEN sum(l.lnpvalor) is null THEN 0 ELSE sum(l.lnpvalor) END as autorizado
				FROM
					academico.lancamentosportaria l
				INNER JOIN academico.portarias p ON p.prtid = l.prtid
				WHERE
					p.prtidautprov = {$prtidautprov}AND
					l.clsid = {$clsid} AND
					l.entidentidade = {$entidade} AND
					l.entidcampus = {$entidcampus} ";

		$resultado = $this->db->pegaUm( $sql );

		return $resultado;

	}

	/**
	 * Função que busca os dados de projetado
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 13/03/2009
	 * @param integer $prtid
	 * @return array
	 */
	function buscaprojetado( $prtid ){

		$sql = "SELECT
					a.clsid as classe,
					coalesce(a.acpvalor, 0) as projetado
				FROM
					academico.acumuladoprojetado a
				WHERE
					a.prtid = {$prtid}";

		$resultado = $this->db->carregar( $sql );

		return $resultado;

	}

	/**
	 * Função que busca os valores projetados de cada classe
	 * em relação aos cargos
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 13/03/2009
	 * @param integer $clsid
	 * @return integer
	 */
	function valoresprojetados( $clsid, $entidcampus, $orgid, $prtid ){

		$entidade = $this->buscaentidade( $entidcampus );

		$sql = "SELECT
					coalesce(sum(ac.acpvalor), 0) as valor
				FROM
					academico.acumuladoprojetado ac
				INNER JOIN
					academico.portarias pr ON ac.prtid = pr.prtid
				WHERE
					ac.clsid = {$clsid} AND
					ac.entidentidade = {$entidade} AND
					ac.entidcampus = {$entidcampus} AND
					ac.prtid = {$prtid}";
		$valor = $this->db->pegaUm( $sql );

		return $valor;
	}

	/**
	 * Enter description here...
	 *
	 * @param unknown_type $clsid
	 * @param unknown_type $entidcampus
	 * @param unknown_type $prtid
	 * @return unknown
	 */
	function valoresprovidos( $clsid, $entidcampus, $prtid ){

		$entidade = $this->buscaentidade( $entidcampus );
		$sql = "SELECT
					COALESCE(SUM(l.lnpvalor),0) as autorizado
				FROM
					academico.lancamentosportaria l
				INNER JOIN academico.portarias p ON p.prtid = l.prtid
				WHERE
					p.prtstatus = 'A' AND
					l.lnpstatus = 'A' AND
					l.prtid = {$prtid} AND
					l.clsid = {$clsid} AND
					l.entidentidade = {$entidade} AND
					l.entidcampus = {$entidcampus} ";
		$resultado = $this->db->pegaUm( $sql );

		return $resultado;
	}

	/**
	 * Função que desenha a tabela com as classes na tela
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 13/03/2009
	 * @param array $autorizado
	 * @param array $projetado
	 * @return mixed
	 */
	function montatabelatecnicos( $entidcampus, $orgid, $prtid ){

		$tprnivel 	 = $_SESSION["academico"]["tprnivel"];
		$cabecalho = "";
		$sql 	   = "";

		$sql = "SELECT
					clsid as id,
					clsdsc as nome
				FROM
					academico.classes
				WHERE
					clsid <> 6
				ORDER BY
					clsid ";

		$dados = $this->db->carregar( $sql );

		// Ações do campos de projetados

		for( $i = 0; $i < count($dados); $i++ ){


			if($tprnivel == ACA_TPORTARIA_PROVIMENTO){

				// Soma os valores projetados para cada classe
				$acpvalor = $this->valoresprojetados($dados[$i]['id'], $entidcampus, $orgid, $_SESSION["academico"]["prtidautprov"]);


				// Soma os valores autorizados para cada classe
				$lnpvalor_con = $this->buscaautorizado( $_SESSION["academico"]["prtidautprov"], $dados[$i]['id'], $entidcampus );

				// recupera o valor lançado
				$lancado = $this->buscalancado( $_SESSION["academico"]["prtidautprov"], $dados[$i]['id'], $entidcampus );

				$saldo = $lnpvalor_con - $lancado;

				// Soma os valores providos para cada classe

				$lnpvalor_prov = $this->valoresprovidos($dados[$i]['id'], $entidcampus, $_SESSION["academico"]["prtid"]);
				$saldo_total = $saldo - $lnpvalor_prov;
				$cabecalho .= "<tr>"
					   . "	<td class='subtitulodireita'>" . (($dados[$i]['nome'] != 'Docentes') ? $dados[$i]['nome'] : $dados[$i]['nome']) . "</td>"
					   . "	<td align='right'> "
					   . "		<input type=\"hidden\" name=\"acpvalor_old[".$dados[$i]['id']."]\" id=\"acpvalor_old\" value=\"" . $acpvalor. "\" >"
					   . "		<input readonly=\"readonly\"  style=\"color:#C0C0C0; type=\"text\" name=\"acpvalor[".$dados[$i]['id']."]\" id=\"acpvalor\" size=\"7\" maxlength=\"4\" value=\"" . $acpvalor . "\" class=\"CampoEstilo\" onkeypress=\"return somenteNumeros(event);\"  onchange=\"validalancamento(" . $dados[$i]['id'] . ")\">"
					   . "	</td>"
					   . "	<td align='right'>"
					   . "		<input type=\"hidden\" name=\"lnpvalor_con_old[".$dados[$i]['id']."]\" id=\"lnpvalor_con_old\" value=\"" . $lnpvalor_con . "\" >"
					   . "		<input readonly=\"readonly\" style=\"color:#C0C0C0; type=\"text\" name=\"lnpvalor_con[".$dados[$i]['id']."]\" id=\"lnpvalor_con\" size=\"7\" maxlength=\"4\" value=\"" . $lnpvalor_con . "\" class=\"normal\" onkeypress=\"return somenteNumeros(event);\" onchange=\"validalancamento(" . $dados[$i]['id'] . ")\">"
					   . "	</td>"

					   . "	<td align='right'>"
					   . "		<input type=\"hidden\" name=\"lnpvalor_prov_old[".$dados[$i]['id']."]\" id=\"lnpvalor_prov_old\" value=\"" . $lnpvalor_prov . "\" >"
					   . "		<input type=\"text\" name=\"lnpvalor_prov[".$dados[$i]['id']."]\" id=\"lnpvalor_prov\" size=\"7\" maxlength=\"4\" value=\"" . $lnpvalor_prov . "\" class=\"normal\" onkeypress=\"return somenteNumeros(event);\" onkeyup=\"validalancamento(" . $dados[$i]['id'] . ")\">"
					   . "	</td>"
					   . "	<td align='right'>"
					   . "		<input type=\"hidden\" name=\"saldoaut_old[".$dados[$i]['id']."]\" id=\"saldoaut_old\" value=\"" . $saldo . "\" >"
					   . "		<input readonly=\"readonly\" style=\"color:#C0C0C0; type=\"text\" name=\"saldoaut[".$dados[$i]['id']."]\" id=\"saldoaut\" size=\"7\" maxlength=\"4\" value=\"" . $saldo . "\" class=\"normal\" onkeypress=\"return somenteNumeros(event);\" onchange=\"validalancamento(" . $dados[$i]['id'] . ")\">"
					   . "	</td>"
					   . "</tr>";
			}else{
				// Soma os valores projetados para cada classe
				$acpvalor = $this->valoresprojetados($dados[$i]['id'], $entidcampus, $orgid, $prtid);


				//atribui os valores do autorizado
				$lnpvalor_con = $this->buscaautorizado( $_SESSION["academico"]["prtid"], $dados[$i]['id'], $entidcampus);

				$cabecalho .= "<tr>"
					   . "	<td class='subtitulodireita'>" . (($dados[$i]['nome'] != 'Docentes') ? $dados[$i]['nome'] : $dados[$i]['nome']) . "</td>"
					   . "	<td align='right'>"
					   . "		<input type=\"text\" name=\"acpvalor[".$dados[$i]['id']."]\" id=\"acpvalor\" size=\"7\" maxlength=\"4\" value=\"" . (!empty($acpvalor) ? $acpvalor : 0) . "\" class=\"CampoEstilo\" onkeypress=\"return somenteNumeros(event);\"  onchange=\"validalancamento(" . $dados[$i]['id'] . ")\">"
					   . "	</td>"
					   . "	<td align='right'>"
					   . "		<input type=\"text\" name=\"lnpvalor_con[".$dados[$i]['id']."]\" id=\"lnpvalor_con\" size=\"7\" maxlength=\"4\" value=\"" . (!empty($lnpvalor_con) ? $lnpvalor_con : 0) . "\" class=\"normal\" onkeypress=\"return somenteNumeros(event);\"  onchange=\"validalancamento(" . $dados[$i]['id'] . ")\">"
					   . "		<input type=\"hidden\" name=\"lnpvalor_con_old[".$dados[$i]['id']."]\" id=\"lnpvalor_con_old\" value=\"" . $lnpvalor_con . "\" >"
					   . "	</td>"
					   . "</tr>";
			}
		}

		return $cabecalho;

	}

	/**
	 * Busca entidade atravéz do campus
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 13/03/2009
	 * @param integer $entidcampus
	 * @return integer
	 */
	function buscaentidade( $entidcampus, $tipoEntidade = '' ){

		if( $tipoEntidade == 'reitoria' ){
			$funid = ACA_ID_REITORIA;
		} else{
			$funid = ACA_ID_CAMPUS;
		}
		// busca a entidade associada ao campus
		$sql = "SELECT ea.entid
				FROM entidade.entidade e
				INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
				INNER JOIN entidade.funentassoc ea ON ef.fueid = ea.fueid
				WHERE e.entid = {$entidcampus} AND ef.funid IN(".ACA_ID_UNIVERSIDADE.",".ACA_ID_ESCOLAS_TECNICAS.",".ACA_ID_UNED.",".ACA_ID_REITORIA.",".$funid.")";
		$entid = $this->db->pegaUm( $sql );

		return $entid;

	}

	/**
	 * Enter description here...
	 *
	 * @param unknown_type $prtid
	 * @return unknown
	 */
	function buscaidprovido( $prtid ){

		$sql_prov = "SELECT prtidautprov FROM academico.portarias WHERE prtid = ".$prtid."";
		$prtidautprov = $this->db->pegaUm($sql_prov);

		return $prtidautprov;

	}

	/**
	 * Enter description here...
	 *
	 * @param unknown_type $prtidautprov
	 * @return unknown
	 */
	function buscaidautorizado( $prtidautprov ){

		$sql_prov = "SELECT prtid FROM academico.portarias WHERE prtidautprov = ".$prtidautprov."";

		$prtid = $this->db->pegaUm($sql_prov);

		return $prtid;

	}

	/**
	 * Enter description here...
	 *
	 * @param unknown_type $prtid
	 * @return unknown
	 */
	function buscatipo ( $prtid ){

		$sql = "SELECT prgid FROM academico.portarias WHERE prtid = {$prtid}";

		$prgid = $this->db->pegaUm( $sql );
		return $prgid;

	}

	/**
	 * Função que lista as portarias cadastradas
	 *
	 * @author Werter Dias Almeida
	 * @since 01/04/2009
	 * @param string $filtroprocesso
	 */
	function listaportarias ($filtroprocesso){
		global $habilitado;
		if ($habilitado){
			$botaoExcluir = "<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"alterar(\'E\','||  p.prtid || ');\" title=Excluir>";
		}else{
			$botaoExcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 ;\" title=Excluir>";
		}
		$sql = "
		SELECT
            '<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' ||  p.prtid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || p.prtid || ' ); abreconteudo(\'academico.php?modulo=principal/listarPortarias&acao=C&subAcao=gravarCarga&carga=' ||  p.prtid || '&params=\' + params, ' ||  p.prtid || ');\"/></center>' as img,
            '<center><img src=\"/imagens/alterar.gif\" style=\"padding-right: 5px; cursor: pointer;\" onclick=\"abreportaria(' || p.prtid|| ');\"border=0 alt=Ir>&nbsp;&nbsp; $botaoExcluir </center>' as acao,
            p.prtid  AS cod,
            '<center>'||p.prtnumero ||'</center>' as numero,
            '<center>'||to_char(p.prtdtinclusao, 'DD/MM/YYYY') ||'</center>' as dtinclusao,
            pg.prgdsc as programa,
            '<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' ||  p.prtid || '\" colspan=\"6\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
		FROM academico.portarias AS p
				INNER JOIN academico.orgao o on o.orgid = p.orgid
				INNER JOIN academico.tipoportaria tp ON tp.tprid = p.tprid
				INNER JOIN academico.programa pg ON pg.prgid = p.prgid
		".((count($filtroprocesso) > 0)?" WHERE prtstatus = 'A' and prtidautprov is null and p.tprid = ".ACA_TPORTARIA_CONCURSO."  and ".implode(" AND ", $filtroprocesso):"");

		$cabecalho = array("Provimentos", "Ações","Nº Controle - Autorização", "Nº Portaria","Data de Inclusão", "Programa");
		$this->db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

	}

	/**
	 * Função que lista as portarias de provimentos cadastradas
	 *
	 * @author Werter Dias Almeida
	 * @since 02/04/2009
	 * @param int $prtid
	 */
	function listaportariasprov ($prtid, $formatoLista = false){

		global $habilitado;
		if ($habilitado){
			$botaoExcluir = "<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"alterar(\'E\','||  p.prtid || ');\" title=Excluir>";
		}else{
			$botaoExcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 ;\" title=Excluir>";
		}

		$cabecalho = array("Nº Controle - Provimento", "Nº Portaria","Data de Inclusão");
		if (!$formatoLista){
			$img = <<<EOT
						'<center><img src=\"/imagens/alterar.gif\" style=\"padding-right: 5px; cursor: pointer;\" onclick=\"abreportaria(' || p.prtid|| ');\"border=0 alt=Ir>&nbsp;&nbsp; $botaoExcluir </center>' as acao,
EOT;
			$programa = ", pg.prgdsc as programa";
			array_unshift($cabecalho, "Ações");
			array_push($cabecalho, "Programa");
		}

		$sql = "
		SELECT
            $img
            p.prtid  AS cod,
            '<center>'||p.prtnumero ||'</center>' as numero,
            '<center>'||to_char(p.prtdtinclusao, 'DD/MM/YYYY') ||'</center>' as dtinclusao
            $programa
        FROM academico.portarias AS p
		INNER JOIN academico.orgao o on o.orgid = p.orgid
		INNER JOIN academico.tipoportaria tp ON tp.tprid = p.tprid
		INNER JOIN academico.programa pg ON pg.prgid = p.prgid
		WHERE prtidautprov = $prtid and prtstatus = 'A'";

		if ($formatoLista)
			$this->db->monta_lista_simples($sql, $cabecalho, 50, 20);
		else
			$this->db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

	}

	/**
	 * Função que lista as portarias de provimentos cadastradas
	 *
	 * @author Werter Dias Almeida
	 * @since 02/04/2009
	 * @param int $prtid
	 */
	function listacampusedital ($entidassociado){

		$sql = "SELECT
					'<a style=\"cursor:pointer;\" onclick=\"abredadoscampus(' || e.entid || ');\">' || upper(e.entnome) || '</a>' as nome
				FROM
					entidade.entidade e2
				INNER JOIN entidade.entidade e ON e2.entid = e.entid
				INNER JOIN entidade.funcaoentidade ef ON e.entid = ef.entid
				INNER JOIN entidade.funentassoc ea ON ef.fueid = ea.fueid
				INNER JOIN academico.entidadeportaria ep ON ea.entid = ep.entid
                INNER JOIN academico.lancamentosportaria lp ON e.entid = lp.entidcampus
				WHERE
					e.entstatus = 'A' AND ea.entid = $entidassociado
				GROUP BY e.entid, e.entnome";

		$cabecalho = array();
		$this->db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

	}

	/**
	 * Função que lista as portarias para o Campus
	 *
	 * @author Werter Dias Almeida
	 * @since 02/04/2009
	 * @param int $prtid
	 */
	function listacampusportarias ($portarias, $ano = ""){

/*		$sql = "
		SELECT DISTINCT
			'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' ||  p.prtid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || p.prtid || ' ); abreconteudo(\'academico.php?modulo=principal/listareditais&acao=C&subAcao=gravarCarga&cargaportarias=' ||  p.prtid || '&params=\' + params, ' ||  p.prtid || ');\"/></center>' as img,
			('<center><img src=\"/imagens/alterar.gif\" style=\"padding-right: 5px; cursor: pointer;\" onclick=\"usaPortaria(' || p.prtid|| ','|| lnp.entidcampus||', '|| p.prgid ||');\"border=0 alt=Usar></center>') as acao,
            p.prtid  AS cod,
			'<center>'||p.prtnumero ||'</center>' as numero,
            '<center>'||to_char(p.prtdtinclusao, 'DD/MM/YYYY') ||'</center>' as dtinclusao,
            pg.prgdsc as programa,
            '<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' ||  p.prtid || '\" colspan=\"6\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
	    FROM academico.portarias AS p
						INNER JOIN academico.orgao o on o.orgid = p.orgid
						INNER JOIN academico.tipoportaria tp ON tp.tprid = p.tprid
						INNER JOIN academico.programa pg ON pg.prgid = p.prgid
		                INNER JOIN academico.lancamentosportaria lnp ON p.prtid = lnp.prtid
		WHERE lnp.entidcampus=$portarias AND p.prtano = '$ano' AND
			  p.tprid = ".ACA_TPORTARIA_CONCURSO." AND
			  p.prtstatus = 'A'
		GROUP BY p.prtid, p.prtnumero, p.prtdtinclusao, pg.prgdsc, lnp.entidcampus, p.prgid";
*/

		$sql = "
		SELECT DISTINCT
            '<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' ||  p.prtid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || p.prtid || ' ); abreconteudo(\'academico.php?modulo=principal/listaPortaria&acao=A&subAcao=gravarCarga&carga=' ||  p.prtid || '\', ' ||  p.prtid || ');\"/></center>' as img,

			('<center><img src=\"/imagens/alterar.gif\" style=\"padding-right: 5px; cursor: pointer;\" onclick=\"usaPortaria(' || p.prtid|| ','|| lnp.entidcampus||', '|| p.prgid ||');\"border=0 alt=Usar></center>') as acao,
            p.prtid  AS cod,
			'<center>'||p.prtnumero ||'</center>' as numero,
            '<center>'||to_char(p.prtdtinclusao, 'DD/MM/YYYY') ||'</center>' as dtinclusao,
            pg.prgdsc as programa,

            '<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' ||  p.prtid || '\" colspan=\"6\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr

	    FROM academico.portarias AS p
						INNER JOIN academico.orgao o on o.orgid = p.orgid
						INNER JOIN academico.tipoportaria tp ON tp.tprid = p.tprid
						INNER JOIN academico.programa pg ON pg.prgid = p.prgid
		                INNER JOIN academico.lancamentosportaria lnp ON p.prtid = lnp.prtid
		WHERE lnp.entidcampus=$portarias AND p.prtano = '$ano' AND
			  p.tprid = ".ACA_TPORTARIA_CONCURSO." AND
			  p.prtstatus = 'A'
		GROUP BY p.prtid, p.prtnumero, p.prtdtinclusao, pg.prgdsc, lnp.entidcampus, p.prgid";


		$cabecalho = array("Provimento", "Ações","Nº Controle", "Nº Portaria","Data de Inclusão", "Programa");
		$this->db->monta_lista_simples($sql, $cabecalho, 50, 20);
//		$this->db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

	}

	/**
	 * Função que lista os editais cadastrados
	 *
	 * @author Werter Dias Almeida
	 * @since 02/04/2009
	 * @param int $prtid
	 */
	function listaeditais ($entidcampus, $filtros, $prtid, $tpeid){

		global $habilitado;

		$sql = "SELECT
					edp.edpid,
					edp.tpeid,
					edp.edpnumero,
					edp.edpdtcriacao,
					edp.edpnumdiario,
					edp.edpdtpubldiario
				FROM
					academico.editalportaria	AS edp
				WHERE
					edp.tpeid=".$tpeid." AND
					edp.prtid = $prtid AND $filtros
					edp.entidcampus = '".$entidcampus."' AND
					edp.edpstatus = 'A' AND
					edp.edpidhomo IS null";

		$dados = $this->db->carregar($sql);

		if( $dados ){
			foreach($dados as $chave => $val){

				$sql="SELECT edpid FROM academico.editalportaria WHERE edpidhomo=".$val['edpid']." AND edpstatus = 'A'";
				$homo= $this->db->pegaUm($sql);
				$habilitado;

				$exluir = $habilitado ? "<a href=# onclick='apagar_edital(".$val['edpid'].")'>
									     <img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>"
									  : "<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\"></a>";

				$acao = "<center><a  href=\"academico.php?modulo=principal/cadedital&acao=C&edpid=".$val['edpid']."&evento=A\">
									<img src=\"/imagens/alterar.gif \" border=0 title=\"Visualizar\"></a>
									$exluir
									</center>
									";

				if ($homo){
					$homologa="<center><a href=academico.php?modulo=principal/cadedital&acao=H&evento=A&edpid=".$homo."> Edital de Homologação - ".$homo."</a></center>";
				} else {
					$homologa="";
				}

				$sql = "
					SELECT
						COALESCE(SUM(publicado), 0) as publicado,
						COALESCE(SUM(homologado), 0) as homologado,
						COALESCE(SUM(lepvlrprovefetivados), 0) as lepvlrprovefetivados
					FROM
					(
							SELECT
								COALESCE(SUM(lep.lepvlrpublicacao), 0) as publicado,
								0 AS homologado,
								0 AS lepvlrprovefetivados
							FROM
								academico.editalportaria ep
								INNER JOIN
									academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
														  AND lep.lepstatus = 'A'
							WHERE
								ep.edpstatus = 'A' AND
								ep.tpeid in ( " . ACA_TPEDITAL_PUBLICACAO . " ) AND
								ep.edpid = {$val['edpid']}

						UNION ALL

							SELECT
								0 AS publicado,
								COALESCE(SUM(lep1.lepvlrhomologado ), 0) as homologado,
								0 AS lepvlrprovefetivados
							FROM
								academico.editalportaria ep1
								INNER JOIN
									academico.lancamentoeditalportaria lep1 ON lep1.edpid = ep1.edpid
														      AND lep1.lepstatus = 'A'
							WHERE
								ep1.edpidhomo = {$val['edpid']}
								AND ep1.edpstatus = 'A'
								AND ep1.tpeid in ( " . ACA_TPEDITAL_HOMOLOGACAO . " )

						UNION ALL

							SELECT
								0 AS publicado,
								0 AS homologado,
								COALESCE(SUM(lep2.lepvlrprovefetivados ), 0) as efetivado
							FROM
								academico.editalportaria ep1
								INNER JOIN
									academico.editalportaria ep2 ON ep2.edpideditalhomologacao = ep1.edpid
								INNER JOIN
									academico.lancamentoeditalportaria lep2 ON lep2.edpid = ep2.edpid
														      AND lep2.lepstatus = 'A'
							WHERE
								ep1.edpidhomo = {$val['edpid']}
								AND ep2.edpstatus = 'A'
								AND ep2.tpeid in ( " . ACA_TPEDITAL_NOMEACAO . " )

					 ) AS foo

				";


				$soma = $this->db->pegaLinha($sql);

				$dados_array[$chave] = array("acao" => $acao,
											 "edpid" 			=> "<center>".$val['edpid']."</center>",
											 "edpnumero" 		=> "<center>".$val['edpnumero']." ",
//				 							 "edpdtcriacao" 	=> "<center>".formata_data($val['edpdtcriacao'])."</center>",
//											 "edpnumdiario" 	=> "<center>".$val['edpnumdiario']."</center>",
				 							 "edpdtpubldiario" 	=> "<center>".formata_data($val['edpdtpubldiario'])."</center>",
											 "totAutorizado"    => ($soma['publicado'] ? $soma['publicado'] : 0),
											 "totHomologado"    => ($soma['homologado'] ? $soma['homologado'] : 0),
											 "totEfetivado"	 	=> ($soma['lepvlrprovefetivados'] ? $soma['lepvlrprovefetivados'] : 0),
											 "edpidLink"		=> $val['edpid']
				);
			}
//			if ($tpeid == ACA_TPEDITAL_NOMEACAO)
//				$cabecalho = array("Ação", "Número de Controle", "Número do Edital", "Data de Publicação", "Número do DOU", "Data de Publicação");
//	   		else
//				$cabecalho = array("Ação", "Número de Controle", "Número do Edital", "Data de Publicação", "Número do DOU", "Data de Publicação", "Edital de Homologação");
			if ($tpeid == ACA_TPEDITAL_NOMEACAO){
				$cabecalho = array("Ação", "Número de Controle", "Número do Edital","Data de Publicação");
			}else{

				$urlAut = "quadro_niveis.php?edpid={campo[8]}&tpeid=" . ACA_TPEDITAL_PUBLICACAO;
				$urlHom = "quadro_niveis.php?edpid={campo[8]}&tpeid=" . ACA_TPEDITAL_HOMOLOGACAO;
				$urlEfe = "quadro_niveis.php?edpid={campo[8]}&tpeid=" . ACA_TPEDITAL_NOMEACAO;

				$cabecalho = array("Ação", "Número de Controle", "Número do Edital","Data de Publicação", "Total Publicado", "Total Homologado", "Total Efetivado");
				$arrHtml   = array(
									"",
									"",
									"",
									"",
									"<div onmousemove=\"SuperTitleAjax( '" . $urlAut . "', this );\"
										  onmouseout=\"SuperTitleOff( this );\"
										  style=\"width:100%; color:#0066CC; text-align:center;\">
									      {campo[5]}
									 </div>",
									"<div onmousemove=\"SuperTitleAjax( '" . $urlHom. "', this );\"
										  onmouseout=\"SuperTitleOff( this );\"
										  style=\"width:100%; color:#0066CC;  text-align:center;\">
									      {campo[6]}
									 </div>",
									"<div onmousemove=\"SuperTitleAjax( '" . $urlEfe . "', this );\"
										  onmouseout=\"SuperTitleOff( this );\"
										  style=\"width:100%; color:#0066CC;  text-align:center;\">
									      {campo[7]}
									 </div>",
									);
	   		}
			$this->db->monta_lista_array($dados_array, $cabecalho, 15, 20, '', 'center', $arrHtml);
		}else{
			$this->db->monta_lista_array("", $cabecalho, 15, 20, '', 'center', '');
		}

	}

	/**
	 * Função que cadastra os valores autorizado e projetado
	 *
	 * @author Fernando Araújo Bagno da Silva
	 * @since 13/03/2009
	 * @param array $dados
	 */
	function cadastraautoriazacaoconcursos( $dados, $orgid, $tprnivel){

		// deleta os dados
		$sql = "DELETE FROM academico.lancamentosportaria WHERE prtid = {$_SESSION["academico"]["prtid"]} AND entidcampus = {$_SESSION["academico"]["entidcampus"]} AND entidentidade = {$_SESSION["academico"]["entid"]}";
			$this->db->executar( $sql );

		if($tprnivel == ACA_TPORTARIA_CONCURSO){
			$sql = "DELETE FROM academico.acumuladoprojetado WHERE prtid = {$_SESSION["academico"]["prtid"]}  AND entidcampus = {$_SESSION["academico"]["entidcampus"]} AND entidentidade = {$_SESSION["academico"]["entid"]}";
			$this->db->executar( $sql );

		}

		// escapa os dados e retira os espaços
		$dados 			   		= $this->quote($dados);
		$dados["lnpvalor_con"] 	= $this->quote($dados["lnpvalor_con"]);
		$dados["lnpvalor_prov"] = $dados["lnpvalor_prov"] ? $this->quote($dados["lnpvalor_prov"]) : '';
		//$dados["acpvalor"] = $this->quote($dados["acpvalor"]);

		if($tprnivel == ACA_TPORTARIA_CONCURSO){

			// insere os valores autorizados para o concurso informado
			if ( is_array($dados["lnpvalor_con"]) ){

				foreach($dados["lnpvalor_con"] as $chave=>$valor){

					$lnpvalor = !empty($valor) ? $valor : '0';

					$unidade = $this->buscatipo( $_SESSION["academico"]["prtid"] );
					$unidade = !empty($unidade) ? $unidade : '';
					$sql = "INSERT INTO academico.lancamentosportaria(prtid, entidcampus,
															  	   	  entidentidade, clsid,
														 		   	  lnpvalor, lnpstatus,
														 		   	  lnpdtinclusao)
							VALUES ({$_SESSION["academico"]["prtid"]}, {$_SESSION["academico"]["entidcampus"]},
									{$_SESSION["academico"]["entid"]}, {$chave},
									{$lnpvalor}, 'A',
									now())";
					$this->db->executar( $sql );

				}
			}
			// insere os valores projetados informados
			if ( is_array($dados["acpvalor"]) ){

					foreach( $dados["acpvalor"] as $chave=>$valor ){

						$acpvalor = !empty($valor) ? $valor : '0';

						$sql = "";
						$sql = "INSERT INTO academico.acumuladoprojetado(prtid,
																		 entidcampus,
																		 entidentidade,
																		 clsid,
																		 acpvalor,
																		 acpstatus,
																		 acpdtinclusao)
								VALUES ({$_SESSION["academico"]["prtid"]},
										{$_SESSION["academico"]["entidcampus"]},
										{$_SESSION["academico"]["entid"]},
										{$chave},
										{$acpvalor},
										'A',
										now())";

						$this->db->executar( $sql );

				}

		}
		}else{
			// insere os valores do provimento informados
			if ( is_array($dados["lnpvalor_prov"]) ){

				foreach($dados["lnpvalor_prov"] as $chave=>$valor){

					$lnpvalor = !empty($valor) ? $valor : '0';
					$unidade = $this->buscatipo( $_SESSION["academico"]["prtid"] );
					$unidade = !empty($unidade) ? $unidade : '';

					$sql = "INSERT INTO academico.lancamentosportaria(prtid, entidcampus,
															  	   	  entidentidade, clsid,
														 		   	  lnpvalor, lnpstatus,
														 		   	  lnpdtinclusao)
							VALUES ({$_SESSION["academico"]["prtid"]}, {$_SESSION["academico"]["entidcampus"]},
									{$_SESSION["academico"]["entid"]}, {$chave},
									{$lnpvalor}, 'A',
									now())";
					$this->db->executar( $sql );

				}
			}
		}

		$this->db->commit( );
		$this->db->sucesso( "principal/autorizacaodeconcursos&acao=C&entid=".$_SESSION["academico"]["entidcampus"]."", '' );

		}


	function listaportariasprovimentos( $prtid, $entidcampus ){

		$sql = "SELECT
					('<center><img src=\"/imagens/alterar.gif\" style=\"padding-right: 5px; cursor: pointer;\" onclick=\"usaPortariaProvimentos(' || p.prtid|| ', ". $entidcampus .", '|| p.prgid ||');\"border=0 alt=Usar></center>') as acao,
					'<center>' ||TO_CHAR(p.prtdtinclusao,'DD/MM/YYYY') ||'</center>' as dtinclusao,
					'<center>' || tp.tprdsc  ||'</center>' as tipoportaria,
					'<center>' || p.prtid ||'</center>' AS cod,
					'<center>' || p.prtnumero ||'</center>' as numero,
					'<center>' ||  u.usunome ||'</center>' as nome
				FROM academico.portarias AS p
					INNER JOIN academico.orgao o on o.orgid = p.orgid
					INNER JOIN academico.tipoportaria tp ON tp.tprid = p.tprid
					INNER JOIN seguranca.usuario u ON u.usucpf = p.usucpf
				WHERE p.prtstatus = 'A' AND
					p.prtidautprov = ".$prtid;

		$cabecalho = array("Ações", "Data de Inclusão", "Tipo de Portaria", "Nº de Controle", "Nº da Portaria", "Responsável");
		$this->db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

	}

}


class cursos{

	public function excluiCursosAjax($curid){
		global $db;

		$sql = "UPDATE
				  academico.curso
				SET
				  curstatus = 'I'
				WHERE
				  curid = $curid";
		$db->executar($sql);
		echo $db->commit();
	}

	public function listaCursosAjax($modalidade, $entidade){
		global $db;

		if($modalidade == '1'){
			$cod_curso = 'Código do Curso';
			$curcod = "CASE WHEN c.curcodinep <> '' THEN c.curcodinep ELSE 'Não Informado' END as codigo_curso";
		}else{
			$cod_curso = 'Código CAPES';
			$curcod = "CASE WHEN c.curcodcapes <> '' THEN c.curcodcapes ELSE 'Não Informado' END as codigo_capes";
		}

		$sql = "SELECT
					( '<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarCurso('|| c.curid ||');\" border=0 alt=\"Ir\" title=\"Alterar\"> ' ||
					         ' <img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiCursos('|| c.curid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao ,
					$curcod,
				    CASE WHEN c.curdsc <> '' THEN c.curdsc ELSE 'Não Informado' END as curso,
				    CASE WHEN c.pgcid is not null THEN pc.pgcdsc ELSE 'Não Informado' END as programa,
				    (CASE WHEN t.turdsc = 'D' THEN 'Diurno'
				    	 WHEN t.turdsc = 'N' THEN 'Noturno'
				    	 ELSE 'Não Informado' END) as TurnoPrevisto,
				    (CASE WHEN tu.turdsc = 'D' THEN 'Diurno'
				    	 WHEN tu.turdsc = 'N' THEN 'Noturno'
				    	 ELSE 'Não Informado' END) as TurnoExcutado,
				    CASE WHEN c.curinicioprev is not null THEN c.curinicioprev ELSE 'Não Informado' END as inicioprev,
				    CASE WHEN c.curinicioexec is not null THEN c.curinicioexec ELSE 'Não Informado' END as inicioexec,
				    sc.stcdsc
				FROM  academico.curso c
				left join academico.programacurso pc
					ON(c.pgcid = pc.pgcid) left join academico.turno t
				    ON(c.turidprevisto = t.turid) left join academico.turno tu
				    ON(c.turidexecutado = tu.turid) left join academico.situacaocurso sc
				    ON(c.stcid = sc.stcid) left join academico.tipocurso tc
				    ON(c.tpcid = tc.tpcid)
				WHERE
					c.tpcid = $modalidade
					and c.entidcampus = $entidade
					and c.curstatus = 'A'
				ORDER BY c.curdsc";

		//monta_titulo( '', '<b>Listagem de Cursos</b>' );

		$cabecalho = array("Ações", $cod_curso, "Curso", "Programa", "Turno Previsto", "Turno Executado", "Inicio Previsto", "Inicio Executado", "Situação");

		return $db->monta_lista($sql, $cabecalho, 15, 4, 'N','Center','', 'form');
	}

	public function verificaCurso($curid){
		global $db;

		$sql = "SELECT
			  turidprevisto,
			  turidexecutado,
			  curcodinep,
			  curcodcapes,
			  tpcid,
			  stcid,
			  pgcid,
			  curdsc,
			  curinicioprev,
			  curinicioexec,
			  curobs
			FROM
			  academico.curso
			WHERE curid = {$curid}";

		return $db->pegaLinha($sql);
	}

	public function insereCurso($request){

		global $db;

		// Atribui valores nulos aos campos em branco
		$request['turidprevisto']  = $request['turidprevisto']  ? $request['turidprevisto']  : 'null';
		$request['turidexecutado'] = $request['turidexecutado'] ? $request['turidexecutado'] : 'null';

		$sql = "INSERT INTO
				  academico.curso(
				  turidprevisto,
				  turidexecutado,
				  curcodinep,
				  curcodcapes,
				  entidcampus,
				  tpcid,
				  stcid,
				  pgcid,
				  curdsc,
				  curinicioprev,
				  curinicioexec,
				  curobs,
				  curdtinclusao,
				  curstatus
				)
				VALUES (
				  {$request['turidprevisto']},
				  {$request['turidexecutado']},
				  '{$request['curcodinep']}',
				  '{$request['curcodcapes']}',
				  {$request['entidcampus']},
				  {$request['tpcid']},
				  {$request['stcid']},
				  ".($request['pgcid'] ? $request['pgcid'] : 'NULL').",
				  '{$request['curdsc']}',
				  '{$request['curinicioprev']}',
				  '{$request['curinicioexec']}',
				  '{$request['curobs']}',
				  '".date('Y-m-d')."',
				  'A'
				)";

		$db->executar($sql);
		return $db->commit();
	}

	public function alteraCurso($request){
		global $db;

		// Atribui valores nulos aos campos em branco
		$request['turidprevisto']  = $request['turidprevisto']  ? $request['turidprevisto']  : 'null';
		$request['turidexecutado'] = $request['turidexecutado'] ? $request['turidexecutado'] : 'null';

		$sql = "UPDATE
				  academico.curso
				SET
				  turidprevisto = {$request['turidprevisto']},
				  turidexecutado = {$request['turidexecutado']},
				  curcodinep = '{$request['curcodinep']}',
				  curcodcapes = '{$request['curcodcapes']}',
				  entidcampus = {$request['entidcampus']},
				  tpcid = {$request['tpcid']},
				  stcid = {$request['stcid']},
				  pgcid = ".($request['pgcid'] ? $request['pgcid'] : 'NULL').",
				  curdsc = '{$request['curdsc']}',
				  curinicioprev = '{$request['curinicioprev']}',
				  curinicioexec = '{$request['curinicioexec']}',
				  curobs = '{$request['curobs']}'
				WHERE
				  curid = {$request['curid']}";
		$db->executar( $sql );
		return $db->commit();
	}

	public function cadastraEdital( $request ){

		global $db;

		$sql = "SELECT exeid FROM academico.execucao
				WHERE curid = {$request["curid"]} AND exeanobase = '{$request["edpano"]}'";
		$exeid = $db->pegaUm($sql);

		if ( !$exeid ){

			$sql = "INSERT INTO academico.execucao(curid, exeanobase)
				    VALUES ({$request["curid"]}, {$request["edpano"]}) RETURNING exeid";
			$exeid = $db->pegaUm($sql);

		}

		$request["edpdtcriacao"]    = $request["edpdtcriacao"]    ? "'" . formata_data_sql( $request["edpdtcriacao"] ) . "'" : 'null';
		$request["edpdtpubldiario"] = $request["edpdtpubldiario"] ? "'" . formata_data_sql( $request["edpdtpubldiario"] ) . "'" : 'null';
		$request["edpnumvagas"]		= $request["edpnumvagas"]     ? $request["edpnumvagas"] : 'null';

		$sql = "INSERT INTO academico.editalcurso(edpnumero,
												  edpdtcriacao,
												  edpnumvagas,
												  edpano,
												  edpnumdiario,
												  edpdtpubldiario,
												  edpsecaodiario,
												  edpdiariopagina,
												  exeid,
												  edpstatus,
												  edpdtinclusao,
												  usucpf)
				VALUES ('{$request["edpnumero"]}',
						{$request["edpdtcriacao"]},
						{$request["edpnumvagas"]},
						'{$request["edpano"]}',
						'{$request["edpnumdiario"]}',
						{$request["edpdtpubldiario"]},
						'{$request["edpsecaodiario"]}',
						'{$request["edpdiariopagina"]}',
						{$exeid},
						'A',
						'now()',
						'{$_SESSION["usucpf"]}') RETURNING edpid";

		$db->executar($sql);
		$db->commit();
		$db->sucesso("principal/execucao_curso");

	}

	public function atualizaEdital( $request, $arquivo ){

		global $db;

		$request["edpdtcriacao"]    = $request["edpdtcriacao"]    ? "'" . formata_data_sql( $request["edpdtcriacao"] ) . "'" : 'null';
		$request["edpdtpubldiario"] = $request["edpdtpubldiario"] ? "'" . formata_data_sql( $request["edpdtpubldiario"] ) . "'" : 'null';
		$request["edpnumvagas"]		= $request["edpnumvagas"]     ? $request["edpnumvagas"] : 'null';

		$sql = "UPDATE
					academico.editalcurso
				SET
					edpnumero = '{$request["edpnumero"]}',
					edpdtcriacao = {$request["edpdtcriacao"]},
					edpnumvagas = {$request["edpnumvagas"]},
					edpano = '{$request["edpano"]}',
					edpnumdiario = '{$request["edpnumdiario"]}',
					edpdtpubldiario = {$request["edpdtpubldiario"]},
					edpsecaodiario = '{$request["edpsecaodiario"]}',
					edpdiariopagina = '{$request["edpdiariopagina"]}'
				WHERE
					edpid = {$request["edpid"]}";

		$db->executar($sql);

		if ( $arquivo["arquivo"]["nome"] ){
			$this->cadastraArquivoEdital($arquivo, $request);
		}

		$db->commit();
		$db->sucesso("principal/execucao_curso");

	}

	public function cadastraArquivoEdital( $arquivo, $request){

		global $db;

		//Insere o registro do arquivo na tabela public.arquivo
		$sql = "INSERT INTO public.arquivo (arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
				VALUES('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$request["arqdescricao"]."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
		$arqid = $db->pegaUm($sql);

		//Insere o registro na tabela obras.arquivosobra
		$sql = "INSERT INTO academico.anexocurso (arqid, edpid, anxstatus, anxdtinclusao)
				VALUES(". $arqid .",{$request["edpid"]}, 'A','now');";
		$db->executar($sql);

		if(!is_dir('../../arquivos/academico/'.floor($arqid/1000))) {
			mkdir(APPRAIZ.'/arquivos/academico/'.floor($arqid/1000), 0777);
		}

		$db->commit();

	}

	public function downloadArquivo( $param ){

		global $db;

		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$param['arqid'];
		$arquivo = current($db->carregar($sql));
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
		if ( !is_file( $caminho ) ) {
			$_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
		}
		$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
		header( 'Content-type: '. $arquivo['arqtipo'] );
		header( 'Content-Disposition: attachment; filename='.$filename);
		readfile( $caminho );
		exit();
	}

	public function excluiDocumento( $request ){

		global $db;

		$sql = "UPDATE academico.anexocurso SET anxstatus = 'I' where anxid=".$request["anxid"];
		$db->executar($sql);

		$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$request["arqid"];
		$db->executar($sql);

		$db->commit();
		$db->sucesso("principal/execucao_curso");

	}

	public function buscaEdital( $edpid ){

		global $db;

		$sql = "SELECT * FROM academico.editalcurso WHERE edpid = {$edpid}";
		return $db->pegaLinha($sql);

	}

	public function excluiEdital( $edpid ){

		global $db;

		$sql = "UPDATE academico.editalcurso SET edpstatus = 'I' WHERE edpid = {$edpid}";
		$db->executar($sql);
		$db->commit();
		$db->sucesso("principal/execucao_curso");

	}

}


class CursosEdital{
	protected $db;

	function __construct(){
		include_once(APPRAIZ. 'includes/classes/DBMontagemValidacao.inc');
		include_once(APPRAIZ. 'includes/classes/DBComando.inc');
		$this->db = new DBComando();
	}

	/*
	 * Função  manterEditalcurso
	 * Método usado para manter (insert/update) os dados da tabela (academico.editalcurso)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    14-10-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterEditalcurso($dados, $where = null){
		$return        = true;
		$tabela   	   = "academico.editalcurso";
		//$atributoWhere = null;
		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"edtid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"entid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"edtobs" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "300",
							"mascara" => null,
							"nulo"    => true,
						),
					"edtdsc" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "100",
							"mascara" => null,
							"nulo"    => false,
						),
					"edttipo" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => true,
						),
					"edtnumero" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "20",
							"mascara" => null,
							"nulo"    => true,
						),
					"edtdtcriacao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => true,
						),
					"edtdtpubldiario" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => true,
						),
					"edtnumdiario" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "20",
							"mascara" => null,
							"nulo"    => true,
						),
					"edtsecaodiario" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "10",
							"mascara" => null,
							"nulo"    => true,
						),
					"edtdiariopagina" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "10",
							"mascara" => null,
							"nulo"    => true,
						),
					"edtano" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "4",
							"mascara" => null,
							"nulo"    => true,
						),
					"edtdtinicioinscricao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => true,
						),
					"edtdtfinalinscricao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => true,
						),
					"edtdtprovainicio" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => true,
						),
					"edtdtprovafinal" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => true,
						),
					"edtdtinicioaulas" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => true,
						),
					"edtstatus" => array(
							"chave"   => null,
							"value"   => 'A',
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"edtdtinclusao" => array(
							"chave"   => null,
							"value"   => date('d-m-Y'),
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => false,
						),
					"usucpf" => array(
							"chave"   => null,
							"value"   => $_SESSION['usucpf'],
							"type"    => "string",
							"tamanho" => "11",
							"mascara" => "cpf",
							"nulo"    => false,
						),
					"edtnumvagas" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$$k = ($atributo->{$k}['chave'] == 'PK') ? $val : $$k;
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value']   = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);
		$edtid = $edtid ? $edtid : $return;

		if (!empty($dados['arquivo']['name']) && $edtid ){
			$arqid  = $this->manterArquivo($dados);
			$return = $arqid;
			if ($return){
				$dados1 = array(
								"edtid" => $edtid,
								"arqid" => $arqid
						 	   );
				$return = $this->manterAnexoedital($dados1);
				if ($return){
					$dados2 = array (
									 "arqid"   => $arqid,
									 "arquivo" => $dados['arquivo']
									);
					$return = $this->uploadArquivo($dados2);
				}
			}
		}

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
		if ($return){
			$this->db->commit();
		}else{
			$this->db->rollback();
		}

		return (($edtid && $return) ? $edtid : $return);
	}

	/*
	 * Função  listaEditalCurso
	 * Método usado para listar os editais de curso(academico.editalcurso)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    15-10-2009
	 * @param    array $param - Pode conter parametros que ajudem na configuração da lista.
	 * @tutorial Array(
				[modulo] => principal/cursosevagas/listaEditaisVagas
				[acao] => A
			)
	 * @return   void
	 */
	function listaEditalCurso($param = null){

		$where = array();
		$modulo = $param['modulo'] ? $param['modulo'] : $_REQUEST['modulo'];
		$acao   = $param['acao'] ? $param['modulo'] : $_REQUEST['acao'];

		$where[] = "entid = " . $_SESSION['academico']['entid'];

		$edttipo = $_SESSION["academico"]["edttipo"] == "T" ? "edttipo = 'T' OR edttipo is null" : "edttipo = '{$_SESSION["academico"]["edttipo"]}'";

		$op = <<<ASDF
		'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Edital" onclick="janela(\'?modulo=principal/cursosevagas/cadCursoVaga&acao=A&edtid=' || edtid || '\', 600, 600, \'cadEditalVaga\');">&nbsp;
		 <img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir Edital" onclick="Excluir(\'?modulo=$modulo&acao=$acao&evento=excluir&edtid=' || edtid || '\', \'Deseja excluir o edital: ' || edtnumero || ' - ' || edtdsc || '?\');">'
ASDF;

		$sql = "SELECT
					$op AS acao,
					edtano,
					edtdsc,
					CASE WHEN edttipo = 'G' THEN 'Graduação'
						 ELSE 'Pós-Graduação' END as tipo,
					edtnumero,
					to_char(edtdtinicioinscricao, 'DD/MM/YYYY') || ' até ' || to_char(edtdtfinalinscricao, 'DD/MM/YYYY') AS inscricao,
					to_char(edtdtprovainicio, 'DD/MM/YYYY') || ' até ' || to_char(edtdtprovafinal, 'DD/MM/YYYY') AS prova,
					edtdtinicioaulas,
					edtnumvagas
				FROM
				    academico.editalcurso
				WHERE
					edtstatus = 'A' AND
					{$edttipo}
					" .  ((count($where) > 0 ? ' AND ' : '') . implode(' AND ', $where));

		$cabecalho = array( "Ação",
							"Ano",
							"Nome do Edital",
							"Tipo de Curso",
							"Nº do Edital",
							"Inscrição",
							"Provas",
							"Início das Aulas",
							"Total de Vagas");

		$this->db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '100%', '' );
	}

	function carregaEdital($edtid){
		if (is_numeric($edtid)){
			$sql = "SELECT
						edtid, ec.entid, edtobs, edtdsc, edttipo, edtnumero, edtdtcriacao, edtdtpubldiario,
					    edtnumdiario, edtsecaodiario, edtdiariopagina, edtano, edtdtinicioinscricao,
					    edtdtfinalinscricao, edtdtprovainicio, edtdtprovafinal, edtdtinicioaulas,
					    edtstatus, edtdtinclusao, edtnumvagas, ec.usucpf, u.usunome
					FROM
						academico.editalcurso ec
					INNER JOIN
						seguranca.usuario u ON u.usucpf = ec.usucpf
					WHERE
					  	   edtid = {$edtid}";

			$arrDados = (array) $this->db->pegaLinha($sql);
		}

		return (array) $arrDados;
	}

	/*
	 * Função  manterAnexoedital
	 * Método usado para manter (insert/update) os dados da tabela (academico.anexoedital)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    16-10-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterAnexoedital($dados, $where = null){
		$return   = true;
		$tabela   = "academico.anexoedital";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"anxid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"arqid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"edtid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"anxstatus" => array(
							"chave"   => null,
							"value"   => 'A',
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"anxdtinclusao" => array(
							"chave"   => null,
							"value"   => date('d-m-Y'),
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
	//	if ($return){
	//		$this->db->commit();
	//	}else{
	//		$this->db->rollback();
	//	}

		return $return;
	}

	function excluirAnexoedital($dados){
		$return = true;
		if ( !empty($dados['anxid']) && !empty($dados['arqid']) ){
			$return = $this->manterAnexoedital(array("anxstatus" => "I"), $dados);
			if ($return){
				$return = $this->manterArquivo(array("arqstatus" => "I"), $dados);
			}
		}else{
			$return = false;
		}

		if ($return){
			$this->db->commit();
		}else{
			$this->db->rollback();
		}
		return $return;
	}

	/*
	 * Função  listaEditalCursoArquivo
	 * Método usado para listar os arquivos vinculados ao edital curso(academico.editalcurso)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    15-10-2009
	 * @param    integer $edtid - Deve conter o ID da tabela (academico.editalcurso)
	 * @return   void
	 */
	function listaAnexoedital($edtid){

		$modulo = $param['modulo'] ? $param['modulo'] : $_REQUEST['modulo'];
		$acao   = $param['acao'] ? $param['modulo'] : $_REQUEST['acao'];


		$op = <<<ASDF
		'<img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir o Arquivo" onclick="Excluir(\'?modulo=$modulo&acao=$acao&evento=EA&edtid=$edtid&arqid=' || ar.arqid || '&anxid=' || anxid || '\', \'Deseja excluir o arquivo: ' || ar.arqnome || '.'|| ar.arqextensao ||'?\');">'
ASDF;

		$sql = "SELECT
					$op AS acao,
					to_char(ax.anxdtinclusao, 'DD/MM/YYYY') AS data,
					'<a style=\"cursor: pointer; color: blue;\" onclick=\"javascript: downloadArquivo(\'DA\', ' || ar.arqid || ', $edtid);\" />' || ar.arqnome || '.'|| ar.arqextensao ||'</a>' as t1,
					round(ar.arqtamanho / 1024) as tamanho,
					ar.arqnome as nome,
					u.usunome as responsavel
				FROM
					academico.anexoedital ax
				INNER JOIN
					public.arquivo ar ON ar.arqid = ax.arqid
										 AND ar.arqstatus = 'A'
				INNER JOIN
					seguranca.usuario u ON u.usucpf = ar.usucpf
				WHERE
					ax.anxstatus = 'A'
					AND ax.edtid" . ($edtid ? " = '$edtid'" : " IS NULL");

		$cabecalho = array( "Ação",
							"Data Inclusão",
							"Nome Arquivo",
							"Tamanho (Kb)",
							"Descrição Arquivo",
							"Responsável");

		$this->db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%', '' );
	}

	/*
	 * Função  manterArquivo
	 * Método usado para manter (insert/update) os dados da tabela (public.arquivo)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    15-10-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterArquivo($dados, $where = null){
		$return   = true;
		$tabela   = "public.arquivo";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"arqid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"arqnome" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "255",
							"mascara" => null,
							"nulo"    => false,
						),
					"arqdescricao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "255",
							"mascara" => null,
							"nulo"    => true,
						),
					"arqextensao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "255",
							"mascara" => null,
							"nulo"    => true,
						),
					"arqtipo" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "255",
							"mascara" => null,
							"nulo"    => false,
						),
					"arqtamanho" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "bigint",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"arqdata" => array(
							"chave"   => null,
							"value"   => date('d-m-Y'),
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => true,
						),
					"arqhora" => array(
							"chave"   => null,
							"value"   => date('H:i:s'),
							"type"    => "string",
							"tamanho" => "8",
							"mascara" => null,
							"nulo"    => true,
						),
					"arqstatus" => array(
							"chave"   => null,
							"value"   => 'A',
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => true,
						),
					"usucpf" => array(
							"chave"   => null,
							"value"   => $_SESSION['usucpf'],
							"type"    => "string",
							"tamanho" => "11",
							"mascara" => "cpf",
							"nulo"    => true,
						),
					"sisid" => array(
							"chave"   => null,
							"value"   => $_SESSION['sisid'],
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}

			if ($dados['arquivo']){
				$atributo->arqnome['value'] 	= current(explode(".", $dados['arquivo']["name"]));
				$atributo->arqextensao['value'] = end(explode(".", $dados['arquivo']["name"]));
				$atributo->arqtipo['value'] 	= $dados['arquivo']["type"] == 'image/pjpeg' ? 'image/jpeg' : $dados['arquivo']["type"];
				$atributo->arqtamanho['value']  = $dados['arquivo']["size"];
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
//		if ($return){
//			$this->db->commit();
//		}else{
//			$this->db->rollback();
//		}

		return $return;
	}

	function uploadArquivo($dados){
		$return = true;

		if (is_array($dados)){
			$arqid = $dados['arqid'];
			if(!is_dir('../../arquivos/academico/')) {
				mkdir(APPRAIZ.'/arquivos/academico/', 0777);
			}
			if(!is_dir('../../arquivos/academico/'.floor($arqid/1000))) {
				mkdir(APPRAIZ.'/arquivos/academico/'.floor($arqid/1000), 0777);
			}

			$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;

			if ( !move_uploaded_file( $dados['arquivo']['tmp_name'], $caminho ) ) {
				echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
				$return = false;
			}
		}
		return $return;
	}

	/*
	 * Função  manterCursodetalhe
	 * Método usado para manter (insert/update) os dados da tabela (academico.cursodetalhe)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    22-10-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterCursodetalhe($dados, $where = null){
		$return   = true;
		$tabela   = "academico.cursodetalhe";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"cdtid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"curid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"turidprevisto" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"turidexecutado" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"cdtcodigoemec" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "15",
							"mascara" => null,
							"nulo"    => true,
						),
					"cdtcodcapes" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "15",
							"mascara" => null,
							"nulo"    => true,
						),
					"stcid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"pgcid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"arcid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"nvcid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"cdtinicioprev" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "4",
							"mascara" => null,
							"nulo"    => true,
						),
					"cdtinicioexec" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "4",
							"mascara" => null,
							"nulo"    => true,
						),
					"cdtobs" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "200",
							"mascara" => null,
							"nulo"    => true,
						),
					"cdtduracao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "numeric",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"cdtpactuacao" => array(
							"chave"   => null,
							"value"   => 'N',
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"cdtdtinclusao" => array(
							"chave"   => null,
							"value"   => date('d-m-Y'),
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => false,
						),
					"cdtstatus" => array(
							"chave"   => null,
							"value"   => 'A',
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"cdtnumvagaprojetada" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "numeric",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"usucpf" => array(
							"chave"   => null,
							"value"   => $_SESSION['usucpf'],
							"type"    => "string",
							"tamanho" => "11",
							"mascara" => "cpf",
							"nulo"    => true,
						),
					"entid" => array(
						"chave"   => "FK",
						"value"   => null,
						"type"    => "integer",
						"tamanho" => null,
						"mascara" => null,
						"nulo"    => true,
					),
					"cdtliberdistribuicao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => true,
						)
				);

		if (!$where['exclusao']){
			$curid = $this->manterCurso($dados, $where);
			if (!$curid){
				return false;
			}
			$dados['curid'] = $curid;
		}

		if ( $dados['entidcampus'] )
			$dados['entid'] = $dados['entidcampus'];

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert

		$return = $this->db->insert($tabela, $atributo, $atributoWhere);

		if( $dados['cdtpactuacao'] == "P" ):

			$cdtidVagas = $dados["cdtid"] ? $dados["cdtid"] : $return;

			$sql = "SELECT cdtid FROM academico.vagaspactuacao WHERE cdtid = {$cdtidVagas}";
			$existeVagas = $this->db->pegaUm( $sql );

			$dados["vgpano2007"] = $dados["vgpano2007"] ? $dados["vgpano2007"] : "null";
			$dados["vgpano2008"] = $dados["vgpano2008"] ? $dados["vgpano2008"] : "null";
			$dados["vgpano2009"] = $dados["vgpano2009"] ? $dados["vgpano2009"] : "null";
			$dados["vgpano2010"] = $dados["vgpano2010"] ? $dados["vgpano2010"] : "null";
			$dados["vgpano2011"] = $dados["vgpano2011"] ? $dados["vgpano2011"] : "null";
			$dados["vgpano2012"] = $dados["vgpano2012"] ? $dados["vgpano2012"] : "null";

			if( !$existeVagas ){
				$sql = "INSERT INTO
							academico.vagaspactuacao( cdtid,
													  vgpano2007,
													  vgpano2008,
													  vgpano2009,
													  vgpano2010,
													  vgpano2011,
													  vgpano2012,
													  vgpstatus,
													  vgpdtinclusao )
											VALUES ( {$cdtidVagas},
													 {$dados["vgpano2007"]},
													 {$dados["vgpano2008"]},
													 {$dados["vgpano2009"]},
													 {$dados["vgpano2010"]},
													 {$dados["vgpano2011"]},
													 {$dados["vgpano2012"]},
													 'A',
													 'now' )";
			}else{

				$sql = "UPDATE
							academico.vagaspactuacao
						SET
							vgpano2007 = {$dados["vgpano2007"]},
						  	vgpano2008 = {$dados["vgpano2008"]},
						  	vgpano2009 = {$dados["vgpano2009"]},
						  	vgpano2010 = {$dados["vgpano2010"]},
						  	vgpano2011 = {$dados["vgpano2011"]},
						  	vgpano2012 = {$dados["vgpano2012"]}
						WHERE
							cdtid = {$cdtidVagas}";

			}

			$this->db->executar( $sql );

		endif;

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
		if ($return){
			$this->db->commit();
		}else{
			$this->db->rollback();
		}



		return $return;
	}

	/*
	 * Função  manterCurso
	 * Método usado para manter (insert/update) os dados da tabela (public.curso)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    22-10-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterCurso($dados, $where = null){
		$return   = true;
		$tabela   = "public.curso";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"curid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"tpcid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"curdsc" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "150",
							"mascara" => null,
							"nulo"    => false,
						),
					"entid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"turid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
				);


		if ( $dados['entidcampus'] )
			$dados['entid'] = $dados['entidcampus'];


		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$$k = ($atributo->{$k}['chave'] == 'PK') ? $val : $$k;
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);
		$curid  = $curid ? $curid : $return;

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
//		if ($return){
//			$this->db->commit();
//		}else{
//			$this->db->rollback();
//		}

		return ($return && $curid) ? $curid : $return;
	}


	/*
	 * Função  listaCursos
	 * Método usado para listar os cursos do campus(academico.cursodetalhe)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    21-10-2009
	 * @param    array $fitro - Pode conter filtros para a lista.
	 * @tutorial Array(
						[tipocurso] => 1
					)
	 * @param    array $param - Pode conter parametros que ajudem na configuração da lista.
	 * @tutorial Array(
						[modulo] => principal/cursosevagas/listaEditaisVagas
						[acao] => A
					)
	 * @return   void
	 */
	function listaCursos(Array $filtro=null, Array $param = null, Array $filtroPesquisa = null ){
		//global $db;
		$sql = "SELECT
					pu.pflcod
				FROM
					seguranca.perfil AS p LEFT JOIN seguranca.perfilusuario AS pu
				  	ON pu.pflcod = p.pflcod
				WHERE
				  	p.sisid = '{$_SESSION['sisid']}'
				  	AND pu.usucpf = '{$_SESSION['usucpf']}'";

		$pflcod = $this->db->pegaUm( $sql );

		$where = array();
		$modulo = $param['modulo'] ? $param['modulo'] : $_REQUEST['modulo'];
		$acao   = $param['acao'] ? $param['modulo'] : $_REQUEST['acao'];

		/*if ( empty($filtro['entid']) && $_SESSION['academico']['entid'] ){
			$filtro['entid'] = $_SESSION['academico']['entid'];
		}*/

		if ( empty($filtro['entidcampus']) && $_SESSION['academico']['entidcampus'] && $param['tipoEntidade'] == 'campus' ){
			$filtro['entidcampus'] = $_SESSION['academico']['entidcampus'];
		}
		if ( empty($filtro['tipocurso']) ){
			$filtro['tipocurso'] = TIPOCURSOGRADUACAO;
		}


		if ( is_array($filtro) ){
			foreach ($filtro AS $k => $val){
				switch ($k){
					/*case 'entid':
						$where[] = "c.entid = $val";
					break;*/
					case 'entidcampus':
						$where[] = "cd.entid = $val";
					break;
					case 'tipocurso':
						if ($val == TIPOCURSOPOSGRADUACAO){
							//$where[] 							= "c.tpcid = " . TIPOCURSOPOSGRADUACAO;
							$campo	 							= "cdtcodcapes";
							$codigo  							= "Código CAPES";
							//$_SESSION['academico']['tipocurso'] = TIPOCURSOPOSGRADUACAO;
						}else{
							//$where[] 							= "c.tpcid = " . TIPOCURSOGRADUACAO;
							$campo	 							= "cdtcodigoemec";
							$codigo 							= "Código e-MEC";
							//$_SESSION['academico']['tipocurso'] = TIPOCURSOGRADUACAO;
						}
					break;
					default:
						$where[] = "$k = '$val'";
				}
			}
		}

		if ( $filtroPesquisa ){

			foreach( $filtroPesquisa as $chave=>$valor ){

				switch( $chave ){

					case "curdsc":
						if ( $valor != null )
							$where[] = "curdsc ilike'%{$valor}%'";
					break;
					case "entidcampus":
						if ( $valor != null )
							$where[] = "cd.entid = {$valor}";
					break;
					case "stcid":
						if ( $valor != null )
							$where[] = "cd.stcid = {$valor}";
					break;
					case "pgcid":
						if ( $valor != null )
							$where[] = "cd.pgcid = {$valor}";
					break;
					case "cdtpactuacao":
						if ( $valor != null )
							$where[] = "cdtpactuacao = '{$valor}'";
					break;
					case "arcid":
						if ( $valor != null )
							$where[] = "arcid = {$valor}";
					break;

				}

			}

		}

		// busca os ids dos campos para filtro na lista
		if( isset($_SESSION['academico']['entid']) ){

			$sql = "SELECT
						fe.entid
					FROM
						entidade.funcaoentidade fe
					INNER JOIN
						entidade.funentassoc fa ON fa.fueid = fe.fueid
					INNER JOIN
						entidade.entidade ee on fe.entid = ee.entid
					WHERE
						fa.entid = {$_SESSION['academico']['entid']} AND
						funid in (17,18)";

			$entidcampus = $this->db->carregarColuna( $sql );

		}


		if ( $param['tipoEntidade'] == 'campus' ){
			$op = <<<ASDF
					'<img src="/imagens/editar_nome.gif" style="cursor:pointer;" border=0 title="Execução do Curso" onclick="window.location = \'?modulo=principal/cursosevagas/execCurso&acao=A&cdtid=' || cdtid || '\';">'

ASDF;
			$where[] = "cd.cdtpactuacao = 'E'";
			$where[] = "cd.cdtliberdistribuicao = 'S'";

		}else{
			$op = "CASE WHEN cdtliberdistribuicao = 'S'
				 	THEN
				 		'<img src=\"/imagens/alterar_01.gif\" border=0 title=\"Foi aprovada a equivalência do curso, não pode ser alterado!\">&nbsp;
				 		 <img src=\"/imagens/excluir_01.gif\" border=0 title=\"Foi aprovada a equivalência do curso, não pode ser excluído!\">&nbsp;'
				 		 || CASE WHEN
				 		 	cd.cdtpactuacao = 'E'
				 		 THEN
				 		 	'<img src=\"/imagens/editar_nome_desabilitada.gif\" border=0 title=\"Foi aprovada a equivalência do curso, não pode ser excluído!\">'
				 		 ELSE
				 		 	'<img src=\"/imagens/consultar_01.gif\" border=0 title=\"Foi aprovada a equivalência do curso, não pode ser excluído!\">'
				 		 END
				 	ELSE
				 		'<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" border=0 title=\"Alterar Curso\" onclick=\"window.location = \'?modulo=principal/cursosevagas/listaCurso&acao=C&tpcid=3&cdtid=' || cdtid || '\';\">&nbsp;";
				 		if ( $pflcod == PERFIL_SUPERUSUARIO || $pflcod == PERFIL_ADMINISTRADOR ){
				 			$op .= "<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" border=0 title=\"Excluir Curso\" onclick=\"Excluir(\'?modulo=$modulo&acao=$acao&evento=excluir&cdtid=' || cdtid || '\', \'Deseja excluir o curso: ' || curdsc || '?\');\">&nbsp;";
				 		}
			$op .= "' END";

		}

//		$op = ($param['tipoEntidade'] == 'campus')
//			?
//				<<<ASDF
//				'<img src="/imagens/editar_nome.gif" style="cursor:pointer;" border=0 title="Execução do Curso" onclick="window.location = \'?modulo=principal/cursosevagas/execCurso&acao=A&cdtid=' || cdtid || '\';">'
//ASDF
//			:
//				<<<ASDF
//				 CASE WHEN cdtliberdistribuicao = 'S'
//				 	THEN
//				 		'<img src="/imagens/alterar_01.gif" style="cursor:pointer;" border=0 title="O curso foi executado, não pode ser alterado!">&nbsp;
//				 		 <img src="/imagens/excluir_01.gif" style="cursor:pointer;" border=0 title="O curso foi executado, não pode ser excluído!">'
//				 	ELSE
//				 		'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Curso" onclick="janela(\'?modulo=principal/cursosevagas/cadCurso&acao=A&cdtid=' || cdtid || '\', 600, 600, \'cadCurso\');">&nbsp;
//				 		 <img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir Curso" onclick="Excluir(\'?modulo=$modulo&acao=$acao&evento=excluir&cdtid=' || cdtid || '\', \'Deseja excluir o curso: ' || curdsc || '?\');">'
//				 END
//ASDF;

		$sql = "SELECT
					'<center>' || $op || '</center>' AS acao,
					$campo,
					curdsc,
					pgcdsc,
					entnome,
					CASE WHEN cdtpactuacao = 'P'
						THEN 'Previsto'
						ELSE 'Executado'
					END AS tipo,
					CASE WHEN cdtpactuacao = 'P'
						THEN tp.turdsc
						ELSE te.turdsc
					END AS turno,
					CASE WHEN cdtpactuacao = 'P'
						THEN cdtinicioprev
						ELSE cdtinicioexec
					END AS cdtinicio

				FROM
				    academico.cursodetalhe cd
				INNER JOIN
					public.curso c ON c.curid = cd.curid
				INNER JOIN
					entidade.entidade e ON e.entid = cd.entid
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
					c.tpcid = {$_SESSION["academico"]["tipocurso"]}
				" .  ((count($where) > 0 ? ' AND ' : '') . implode(' AND ', $where)) . "
				" . ( $entidcampus ? "AND cd.entid in (" . implode( ",", $entidcampus ) . ")" : "" ) . "
				ORDER BY
					curdsc";

		$cabecalho = array( "Ação",
							$codigo,
							"Curso",
							"Programa",
							"Campus",
							"Tipo",
							"Turno",
							"Início");

		$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N');
	}


	function excluiCurso( $cdtid ){

		$sql = "UPDATE academico.cursodetalhe SET cdtstatus = 'I' where cdtid = {$cdtid}";
		$this->db->executar( $sql );
		$this->db->commit( );
		$this->db->sucesso( "principal/cursosevagas/listaCurso", "" );

	}

	/*
	 * Função  listaCadExecCurso
	 * Método usado para listar os editais de curso(academico.editalcurso)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    28-10-2009
	 * @param    array $fitro - Pode conter filtros para a lista.
	 * @tutorial Array(
						[tipocurso] => 1
					)
	 * @param    array $param - Pode conter parametros que ajudem na configuração da lista.
	 * @tutorial Array(
						[modulo] => principal/cursosevagas/listaEditaisVagas
						[acao] => A
					)
	 * @return   void
	 */
	function listaCadExecCurso($filtro=null, $param = null){
		$where 	   = array();
		$whereExec = array();

		if ($filtro['edtano']){
			$where[] = "edtano = '" . $filtro['edtano'] . "'";
		}

		if ($filtro['entid']){
			$where[] = "entid = " . $filtro['entid'];
		}else{
			$where[] = "entid = " . $_SESSION['academico']['entid'];
		}

		if ($filtro['excid']){
			$where[] 	= "exc.excid = {$filtro['excid']}";
			$inputValue = "' || exc.excnumvagas || '";
		}else{
			$where[] = "exc.edtid IS NULL";
		}

		if ($filtro['cdtid']){
			$whereExec[] = "exc.cdtid = " . $filtro['cdtid'];
		}else{
			$whereExec[] = "exc.cdtid = " . $_SESSION['academico']['cdtid'];
		}

		$campo = <<<ASDF
		'<input type="hidden" name="edtid[]" value="' || ec.edtid ||'"/>&nbsp;
		 <input type="text" name="excnumvagas[' || ec.edtid || ']" size="7" maxlength="10" value="$inputValue" onKeyUp= "this.value=mascaraglobal(\'##########\',this.value);"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:9ex;" title="Nº de Vagas" />&nbsp;
		 <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>'
ASDF;

		$sql = "SELECT
					edtnumero,
					'<div onmousemove=\"SuperTitleAjax( \'academico.php?modulo=" . $_REQUEST['modulo'] . "&acao=" . $_REQUEST['acao'] . "&edtid=' || ec.edtid || '&evento=supertitle\', this );\"
						 onmouseout=\"SuperTitleOff( this );\"
						 style=\"width:100%; color:#0066CC; text-align:center;\">
							' || edtdsc || '
					 </div>' AS edtdsc,
					$campo AS campo
				FROM
				    academico.editalcurso ec
				LEFT JOIN
					academico.execucaocurso exc ON exc.edtid = ec.edtid
												   AND exc.excstatus = 'A'
												   " .  ((count($whereExec) > 0 ? ' AND ' : '') . implode(' AND ', $whereExec)) . "
				WHERE
					edtstatus = 'A'
				" .  ((count($where) > 0 ? ' AND ' : '') . implode(' AND ', $where));

		$cabecalho = array( "Nº do Edital",
							"Nome do Edital",
							"Nº de Vagas");

		$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N');
	}

	function listaCadExecCurso2010($filtro=null, $param = null){
		$where 	   = array();
		$whereEx   = array();
		$whereExec = array();

		if ($filtro['edtano']){
			$where[] = "edtano = '" . $filtro['edtano'] . "'";
		}

		if ($filtro['entid']){
			$where[] = "entid = " . $filtro['entid'];
		}else{
			$where[] = "entid = " . $_SESSION['academico']['entid'];
		}

		if ($filtro['excid']){
			$where[] 	= "exc.excid = {$filtro['excid']}";
			$inputValue = "' || exc.excnumvagas || '";
		}else{
			$where[] = "exc.edtid IS NULL";
		}

		if ($filtro['cdtid']){
			$whereExec[] = "exc.cdtid = " . $filtro['cdtid'];
			$whereEx[]   = "exe.cdtid = " . $filtro['cdtid'];
		}else{
			$whereExec[] = "exc.cdtid = " . $_SESSION['academico']['cdtid'];
			$whereEx[]   = "exe.cdtid = " . $_SESSION['academico']['cdtid'];
		}

		$campo = <<<ASDF
		'<input type="hidden" name="edtid[]" value="' || ec.edtid ||'"/>&nbsp;
		 <input type="text" name="excnumvagas[' || ec.edtid || ']" size="7" maxlength="10" value="$inputValue" onKeyUp= "this.value=mascaraglobal(\'##########\',this.value);"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:9ex;" title="Nº de Vagas" />&nbsp;
		 <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>'
ASDF;

		$sql = "SELECT
					case when ec.edtid = exe.edtid then
						'<center><input type=\"radio\" checked=checked id=\"editalCurso\" name=\"edital\" value=\"'|| ec.edtid ||'\"></center>'
					else
						'<center><input type=\"radio\" id=\"editalCurso\" name=\"edital\" value=\"'|| ec.edtid ||'\"></center>'
					end as acao,
					edtnumero,
					'<div onmousemove=\"SuperTitleAjax( \'academico.php?modulo=" . $_REQUEST['modulo'] . "&acao=" . $_REQUEST['acao'] . "&edtid=' || ec.edtid || '&evento=supertitle\', this );\"
						 onmouseout=\"SuperTitleOff( this );\"
						 style=\"width:100%; color:#0066CC; text-align:center;\">
							' || edtdsc || '
					 </div>' AS edtdsc
				--	edtdsc
				FROM
				    academico.editalcurso ec
				LEFT JOIN
					academico.execucaocurso exc ON exc.edtid = ec.edtid
												   AND exc.excstatus = 'A'
												   " .  ((count($whereExec) > 0 ? ' AND ' : '') . implode(' AND ', $whereExec)) . "
				LEFT JOIN
					academico.editalexecucao exe ON exe.edtid = ec.edtid
													" .  ((count($whereEx) > 0 ? ' AND ' : '') . implode(' AND ', $whereEx)) . "
				WHERE
					edtstatus = 'A'
				" .  ((count($where) > 0 ? ' AND ' : '') . implode(' AND ', $where));


		$cabecalho = array( "Ação",
							"Nº do Edital",
							"Nome do Edital");

		$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N');
	}


	/*
	 * Função  listaExecCursos
	 * Método usado para listar a execução dos cursos do campus(academico.execucaocurso)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    28-10-2009
	 * @param    array $fitro - Pode conter filtros para a lista.
	 * @tutorial Array(
						[tipocurso] => 1
					)
	 * @param    array $param - Pode conter parametros que ajudem na configuração da lista.
	 * @tutorial Array(
						[modulo] => principal/cursosevagas/listaEditaisVagas
						[acao] => A
					)
	 * @return   void
	 */
	function listaExecCursos($filtro=null, $param = null){
		$where = array();
		$modulo = $param['modulo'] ? $param['modulo'] : $_REQUEST['modulo'];
		$acao   = $param['acao'] ? $param['modulo'] : $_REQUEST['acao'];

		$where[] = !empty($filtro['cdtid']) ? "ex.cdtid = {$filtro['cdtid']}" : "ex.cdtid = {$_SESSION['academico']['cdtid']}";

		if ($param['listadetalhe']){
			if ( !empty($filtro['ano']) ){
				$where[] = "ex.excanoexecucao = '{$filtro['ano']}'";
			}

			$op = <<<ASDF
			'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Execução" onclick="janela(\'?modulo=principal/cursosevagas/cadCursoOferta&acao=A&excid=' || ex.excid || '\', 600, 480, \'cadCurso\');">&nbsp;
			 <img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir Execução" onclick="Excluir(\'?modulo=$modulo&acao=$acao&evento=excluirExec&excid=' || ex.excid || '\', \'Deseja excluir a execução do edital de curso de Nº: ' || edtnumero || '?\');">'
ASDF;
			$sql = "SELECT
						$op AS acao,
						'<div onmousemove=\"SuperTitleAjax( \'academico.php?modulo=" . $_REQUEST['modulo'] . "&acao=" . $_REQUEST['acao'] . "&edtid=' || ec.edtid || '&evento=supertitle\', this );\"
							  onmouseout=\"SuperTitleOff( this );\"
							  style=\"width:100%; color:#0066CC; text-align:center;\">
								' || edtnumero || '
						 </div>' AS edtnumero,
						edtdtpubldiario,
						excnumvagas,
						usunome
					FROM
						academico.execucaocurso ex
					INNER JOIN
						academico.editalcurso ec ON ec.edtid = ex.edtid
													AND ec.edtstatus = 'A'
					INNER JOIN
						seguranca.usuario u ON u.usucpf = ex.usucpf
					WHERE
						ex.excstatus = 'A'
					" .  ((count($where) > 0 ? ' AND ' : '') . implode(' AND ', $where)) . "
					ORDER BY
						edtdtpubldiario";

			$cabecalho = array( "Ação",
								"Nº Edital",
								"Data do Edital",
								"Nº de Vagas",
								"Inserido Por"
							  );
		}else{
			$op = <<<ASDF
			'<center>
				<img src="../imagens/mais.gif" style="padding-right: 5px; cursor: pointer;" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img' || ex.excanoexecucao || '" name="+" onclick="abreconteudo(\'academico.php?modulo=principal/cursosevagas/execCurso&acao=A&evento=listaExecCursos&listadetalhe=true&ano=' || ex.excanoexecucao || '\', \'' || ex.excanoexecucao || '\');"/>
			 </center>'
ASDF;
			$tr = <<<ASDF
			'<tr>
				<td style="padding:0px;margin:0;"></td>
				<td id="td' || ex.excanoexecucao || '" colspan="10" style="padding:0px;display:none;border: 5px red"></td>
			 </tr>'
ASDF;

			$sql = "SELECT
						$op AS editais,
						ex.excanoexecucao,
						pjvnumvagas,
						SUM(excnumvagas) AS vagasofertadas,
						pifvagasvest,
						pifingressantes,
						pifmatriculados,
						pifconcluintes,
						$tr AS tr
					FROM
						academico.cursodetalhe cd
					INNER JOIN
					    academico.execucaocurso ex ON ex.cdtid = cd.cdtid
					    							  AND ex.excstatus = 'A'
					INNER JOIN
					    academico.editalcurso edc ON edc.edtid = ex.edtid
					    							 AND edc.edtstatus = 'A'
					LEFT JOIN
						academico.projecaovagas pv ON pv.cdtid = cd.cdtid
													  AND pv.pjvanobase = ex.excanoexecucao
					LEFT JOIN
						academico.pingifes pi ON pi.cdtid = cd.cdtid
												 AND pi.pifanobase = ex.excanoexecucao
					WHERE
						cdtstatus = 'A'
					" .  ((count($where) > 0 ? ' AND ' : '') . implode(' AND ', $where)) . "
					GROUP BY
						editais,
						ex.excanoexecucao,
						pjvnumvagas,
						pifvagasvest,
						pifingressantes,
						pifmatriculados,
						pifconcluintes
					ORDER BY
						ex.excanoexecucao";

			$cabecalho = array( "Editais",
								"Ano Base",
								"Vagas Projetadas",
								"Vagas Ofertadas",
								"<b>Vagas - PINGIFES</b>",
								"<b>Ingressos - PINGIFES</b>",
								"<b>Matriculas - PINGIFES</b>",
								"<b>Concluintes - PINGIFES</b>",
							  );
		}

		$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N');
	}

	function carregaCurso($cdtid){
		if (is_numeric($cdtid)){
			$sql = "SELECT
						cd.cdtid, turidprevisto, turidexecutado, cdtcodigoemec, cdtcodcapes,
					    stcid, pgcid, arcid, cdtinicioprev, cdtinicioexec, cdtobs, cdtduracao,
					    cdtpactuacao, c.curid, tpcid, curdsc, c.entid AS instituicao, cd.entid AS entidCampus, turid, nvcid,
					    vgpano2007, vgpano2008, vgpano2009, vgpano2010, vgpano2011, vgpano2012
					FROM
						public.curso c
					INNER JOIN
						academico.cursodetalhe cd ON cd.curid = c.curid
					LEFT JOIN
						academico.vagaspactuacao av ON av.cdtid = cd.cdtid
					WHERE
						cdtstatus = 'A'
					  	AND cd.cdtid = {$cdtid}";

			$arrDados = (array) $this->db->pegaLinha($sql);
		}

		return (array) $arrDados;
	}

	/*
	 * Função  manterExecucaocurso
	 * Método usado para manter (insert/update) os dados da tabela (academico.execucaocurso)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    29-10-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterExecucaocurso($dados, $where = null){
		$return   = true;
		$tabela   = "academico.execucaocurso";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"excid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"cdtid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"edtid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"usucpf" => array(
							"chave"   => null,
							"value"   => $_SESSION['usucpf'],
							"type"    => "string",
							"tamanho" => "11",
							"mascara" => "cpf",
							"nulo"    => false,
						),
					"excnumvagas" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "numeric",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"excanoexecucao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "4",
							"mascara" => null,
							"nulo"    => false,
						),
					"excstatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"excdtinclusao" => array(
							"chave"   => null,
							"value"   => date('d-m-Y'),
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
		if ($return){
			$this->db->commit();
		}else{
			$this->db->rollback();
		}

		return $return;
	}

	function carregaExecucaocurso($filtro){
		if (is_numeric($filtro['excid']) && is_numeric($filtro['cdtid'])){
			$sql = "SELECT
						excnumvagas,
						excanoexecucao
					FROM
						academico.execucaocurso
					WHERE
						excstatus = 'A'
					  	AND excid = {$filtro['excid']}
					  	AND cdtid = {$filtro['cdtid']}";

			$arrDados = (array) $this->db->pegaLinha($sql);
		}

		return (array) $arrDados;
	}

	function cabecalhoCurso($cdtid){
		$orgid 		 = $_SESSION['academico']['orgid'];
		$entid 		 = $_SESSION['academico']['entid'];
		$entidcampus = $_SESSION['academico']['entidcampus'];

		if ($cdtid && $entid && $entidcampus && $orgid){
			if($orgid == ACA_ORGAO_SUPERIOR){
				$orgao = "Educação Superior";
			}else{
				$orgao = "Educação Profissional";
			}

			$nomeInstituicao = $this->db->pegaUm( "SELECT entnome FROM entidade.entidade WHERE entid = {$entid}" );

			$sql = "SELECT
						e.entnome AS campus,
						en.estuf || ' / ' || m.mundescricao AS endereco,
						c.curdsc,
						CASE WHEN TRIM(t.turdsc) != ''
							THEN t.turdsc
							ELSE 'Não informado'
						END AS turno,
						CASE WHEN TRIM(cd.cdtinicioexec) != ''
							THEN cd.cdtinicioexec
							ELSE 'Não informado'
						END AS inicioexec
					FROM
						academico.cursodetalhe cd
					INNER JOIN
						entidade.entidade e ON e.entid = cd.entid
								      		   AND e.entid = {$entidcampus}
					INNER JOIN
						entidade.endereco en ON en.entid = e.entid
					INNER JOIN
						territorios.municipio m ON m.muncod = en.muncod
					INNER JOIN
						public.curso c ON c.curid = cd.curid
					LEFT JOIN
						academico.turno t ON t.turid = cd.turidexecutado
					WHERE
						cd.cdtid = {$cdtid}";

					$d = $this->db->pegaLinha($sql);
					if ($d){
						$cabecalho = "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>"
									. "	<tr>"
									. "		<td class='SubTituloDireita' width='250px;'>Tipo Ensino:</td><td>".$orgao."</td>"
									. "	</tr>"
									. "	<tr>"
									. "		<td class='SubTituloDireita'>Instituição:</td><td>".$nomeInstituicao."</td>"
									. "	</tr>"
									. "	<tr>"
									. "		<td class='SubTituloDireita'>Campus / Uned:</td><td>".$d['campus']."</td>"
									. "	</tr>"
									. "	<tr>"
									. "		<td class='SubTituloDireita'>UF / Munícipio:</td><td>" . $d['endereco'] . " </td>"
									. "	</tr>"
									. "	<tr>"
									. "		<td class='SubTituloDireita'>Curso:</td><td>" . $d['curdsc'] . " </td>"
									. "	</tr>"
									. "	<tr>"
									. "		<td class='SubTituloDireita'>Turno:</td><td>" . $d['turno'] . " </td>"
									. "	</tr>"
									. "	<tr>"
									. "		<td class='SubTituloDireita'>Ano de Início de Funcionamento:</td><td>" . $d['inicioexec'] . " </td>"
									. "	</tr>"
									. "</table>";
					}else {
						$cabecalho =("<script>
										alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
										window.location='?modulo=inicio&acao=C';
				 					  </script>");
					}
		}else{
						$cabecalho =("<script>
										alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
										window.location='?modulo=inicio&acao=C';
				 					  </script>");
		}
		return $cabecalho;
	}

	/*
	 * Função  listaExecCursos
	 * Método usado para listar a execução dos cursos do campus(academico.execucaocurso)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    28-10-2009
	 * @param    array $fitro - Pode conter filtros para a lista.
	 * @tutorial Array(
						[tipocurso] => 1
					)
	 * @param    array $param - Pode conter parametros que ajudem na configuração da lista.
	 * @tutorial Array(
						[modulo] => principal/cursosevagas/listaEditaisVagas
						[acao] => A
					)
	 * @return   void
	 */
	function listaEquivalencia($filtro=null, $param = null){
		$where 				= array();
		$modulo 			= $param['modulo'] ? $param['modulo'] : $_REQUEST['modulo'];
		$acao   			= $param['acao'] ? $param['modulo'] : $_REQUEST['acao'];
		$param['tipoLista'] = empty($param['tipoLista']) ? 'pactuado' : $param['tipoLista'];


		// busca os ids dos campos para filtro na lista
		if( isset($_SESSION['academico']['entid']) ){

			$sql = "SELECT
						fe.entid
					FROM
						entidade.funcaoentidade fe
					INNER JOIN
						entidade.funentassoc fa ON fa.fueid = fe.fueid
					INNER JOIN
						entidade.entidade ee on fe.entid = ee.entid
					WHERE
						fa.entid = {$_SESSION['academico']['entid']} AND
						funid in (17,18)";

			$entidcampus = $this->db->carregarColuna( $sql );

		}

		if ( is_array($filtro) ){
			foreach ($filtro AS $k => $val){
				switch ($k){
					case 'entid':
						$where[] = "ge.entid = $val";
					break;
					default:
						$where[] = "$k = '$val'";
				}
			}
		}

//		if ($param['listadetalhe']){
			$op = <<<ASDF
			'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Equivalência" onclick="redireciona(\'?modulo=principal/cursosevagas/cadEquivalencia&acao=A&greid=' || ge.greid || '\')">&nbsp;' ||
			 CASE WHEN ec.excid IS NULL
			 	THEN '<img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir Equivalência" onclick="Excluir(\'?modulo=$modulo&acao=$acao&evento=excluirEquiv&greid=' || ge.greid || '\', \'Deseja excluir a equivalência?\');">'
			 	ELSE '<img src="/imagens/excluir_01.gif" style="cursor:pointer;" border=0 title="Não é possível excluir a equivalência que foi Executada">'
			 END
ASDF;

			$sql = "SELECT
						DISTINCT
						$op AS acao,
						ce.greid,
						ce.curidpactuado,
						c.curdsc AS pactuado,
						e.entnome AS campus_pactuado,
						CASE WHEN cd.turidexecutado is not null THEN tp.turdsc ELSE ' - ' END AS turno_pactuado,
						cd.cdtinicioexec AS inicio_pactuado,
						ce.curidexecutado,
						c1.curdsc AS executado,
						e1.entnome AS campus_executado,
						CASE WHEN cd1.turidexecutado is not null THEN te.turdsc ELSE ' - ' END AS turno_executado,
						cd1.cdtinicioexec AS inicio_executado,
						ed.esddsc
					FROM
						academico.grupoequivalencia ge
					INNER JOIN
						workflow.documento d ON d.docid = ge.docid
					INNER JOIN
						workflow.estadodocumento ed ON ed.esdid = d.esdid

					INNER JOIN
						academico.cursoequivalencia ce ON ce.greid = ge.greid

					INNER JOIN
						public.curso c ON c.curid = ce.curidpactuado
										  $whereCurso
					INNER JOIN
						academico.cursodetalhe cd ON cd.curid = c.curid
													 AND cd.cdtstatus = 'A'
					LEFT JOIN
						academico.turno tp ON tp.turid = cd.turidexecutado
					INNER JOIN
						entidade.entidade e ON e.entid = cd.entid

					INNER JOIN
						public.curso c1 ON c1.curid = ce.curidexecutado
										   $whereCurso1
					INNER JOIN
						academico.cursodetalhe cd1 ON cd1.curid = c1.curid
													  AND cd1.cdtstatus = 'A'
					LEFT JOIN
						academico.turno te ON te.turid = cd1.turidexecutado
					INNER JOIN
						entidade.entidade e1 ON e1.entid = cd1.entid
					LEFT JOIN
						academico.execucaocurso ec ON ec.cdtid = cd1.cdtid
													  AND ec.excstatus = 'A'
					WHERE
						c.tpcid = {$_SESSION["academico"]["tipocurso"]} " . ( implode(" AND ", $where) ) .
						( $entidcampus ? " AND cd.entid in (" . implode( ",", $entidcampus ) . ")" : "" ) . "

					ORDER BY
						  pactuado, campus_pactuado";


			$dados = $this->db->carregar( $sql );
			$dados = is_array($dados) ? $dados : array();


			$arrLinha = array();
			$reg 	  = array();
			$linha	  = array();

			foreach ($dados as $d):

				if ( $reg['greid'] != $d['greid'] && isset($reg['greid']) ){
					array_unshift($linha, $reg['acao']);
					array_push($arrLinha, $linha);

					$linha = array();
				}

				if ( $param['tipoLista'] == 'pactuado' ):

					if ( $reg['curidpactuado'] == $d['curidpactuado'] ){
						$linha['pactuado'] 		  = $d['pactuado'];
						$linha['campus_pactuado'] = $d['campus_pactuado'];
						$linha['turno_pactuado']  = $d['turno_pactuado'];
						$linha['inicio_pactuado'] = $d['inicio_pactuado'];
					}else{
						$linha['pactuado'] 		  .= (empty($linha['pactuado']) ? ' ' : '<br/> ') . $d['pactuado'];
						$linha['campus_pactuado'] .= (empty($linha['campus_pactuado']) ? ' ' : '<br/> ') . $d['campus_pactuado'];
						$linha['turno_pactuado']  .= (empty($linha['turno_pactuado']) ? ' ' : '<br/> ') . $d['turno_pactuado'];
						$linha['inicio_pactuado'] .= (empty($linha['inicio_pactuado']) ? ' ' : '<br/> ') . $d['inicio_pactuado'];
					}

					if ( $reg['curidexecutado'] == $d['curidexecutado'] ){
						$linha['executado'] 	   = $d['executado'];
						$linha['campus_executado'] = $d['campus_executado'];
						$linha['turno_executado']  = $d['turno_executado'];
						$linha['inicio_executado'] = $d['inicio_executado'];
					}else{
						$linha['executado'] 	   .= (empty($linha['executado']) ? ' ' : '<br/> ') . $d['executado'];
						$linha['campus_executado'] .= (empty($linha['campus_executado']) ? ' ' : '<br/> ') . $d['campus_executado'];
						$linha['turno_executado']  .= (empty($linha['turno_executado']) ? ' ' : '<br/> ') . $d['turno_executado'];
						$linha['inicio_executado'] .= (empty($linha['inicio_executado']) ? ' ' : '<br/> ') . $d['inicio_executado'];
					}

				else:

					if ( $reg['curidexecutado'] == $d['curidexecutado'] ){
						$linha['executado'] 	   = $d['executado'];
						$linha['campus_executado'] = $d['campus_executado'];
					}else{
						$linha['executado'] 	   .= (empty($linha['executado']) ? ' ' : '<br/> ') . $d['executado'];
						$linha['campus_executado'] .= (empty($linha['campus_executado']) ? ' ' : '<br/> ') . $d['campus_executado'];
					}

					if ( $reg['curidpactuado'] == $d['curidpactuado'] ){
						$linha['pactuado'] 		  = $d['pactuado'];
						$linha['campus_pactuado'] = $d['campus_pactuado'];
					}else{
						$linha['pactuado'] 		  .= (empty($linha['pactuado']) ? ' ' : '<br/> ') . $d['pactuado'];
						$linha['campus_pactuado'] .= (empty($linha['campus_pactuado']) ? ' ' : '<br/> ') . $d['campus_pactuado'];
					}

				endif;

				$linha['estadoDoc']    = $d['esddsc'];

				$linha['turno_executado'] 		   = $d['turno_executado'];
				$linha['inicio_executado'] 		   = $d['inicio_executado'];

				$reg['curidpactuado']  = $d['curidpactuado'];
				$reg['curidexecutado'] = $d['curidexecutado'];
				$reg['acao'] 		   = $d['acao'];
				$reg['greid'] 		   = $d['greid'];

			endforeach;
			if ( is_array( $arrLinha ) && is_array( $linha ) ){
				array_unshift($linha, $reg['acao']);
				array_push($arrLinha, $linha);
			}

			if ( $param['tipoLista'] == 'pactuado' ):
				$cabecalho = array(
									"Ação",
									"Cursos Previstos",
									"Campus",
									"Turno",
									"Inicio",
									"Execução",
									"Campus",
									"Turno",
									"Inicio"
								   );
			else:
				$cabecalho = array(
									"Ação",
									"Execução",
									"Campus",
									"Cursos Previstos",
									"Campus",
								   );
			endif;

			array_push($cabecalho, "Situação");

			if( $arrLinha[0][0] == "" ){
				$cabecalho = array();
				$arrLinha[0][0] = "<span style='color:#cc0000; text-align:center;'><center>Não foram encontratos Registros.</center></span>";
			}

			$this->db->monta_lista_simples( $arrLinha, $cabecalho, 100, 30, 'N');

	}

	function listaCheckCursoEquivalencia($filtro=null, Array $param = null){
		$where 		= array();
		$grupoEquiv = array();

		// busca os ids dos campos para filtro na lista
		if( isset($_SESSION['academico']['entid']) ){

			$sql = "SELECT
						fe.entid
					FROM
						entidade.funcaoentidade fe
					INNER JOIN
						entidade.funentassoc fa ON fa.fueid = fe.fueid
					INNER JOIN
						entidade.entidade ee on fe.entid = ee.entid
					WHERE
						fa.entid = {$_SESSION['academico']['entid']} AND
						funid in (17,18)";

			$entidcampus = $this->db->carregarColuna( $sql );

		}

		//( $entidcampus ? " AND c.entid in (" . implode( ",", $entidcampus ) . ")" : "" )

		if ( is_array($filtro) ){
			foreach ($filtro AS $k => $val){
				switch ($k){
					case 'entid':
						$where[] = "c.entid = $val";
					break;
					case 'greid':
						$grupoEquiv = $this->carregaGrupoCursoEquivalencia( array("greid" => $val), array("tipoEquivalencia" => $param['tipoLista']) );
					break;
					default:
						$where[] = "$k = '$val'";
				}
			}
		}

//		dbg( $grupoEquiv, 1 );

		if ( $param['tipoLista'] == 'E' ){
			$nome = 'curidexecutado';
			$func = 'montaExecutados(this)';
			array_push($where, "cd.cdtpactuacao = 'E'");
			$subWhere = ( count( $grupoEquiv ) ) ? "WHERE curidexecutado NOT IN(" . ( implode(" , ", $grupoEquiv) ) . ")" : "";
			array_push($where, "c.curid NOT IN (SELECT
													curidexecutado
												FROM
													academico.cursoequivalencia
												$subWhere)");
		}else{
			$nome = 'curidpactuado';
			$func = 'montaPactuados(this)';
			array_push($where, "cd.cdtpactuacao = 'P'");
			$subWhere = ( count( $grupoEquiv ) ) ? "WHERE curidpactuado NOT IN(" . ( implode(" , ", $grupoEquiv) ) . ")" : "";
			array_push($where, "c.curid NOT IN (SELECT
													curidpactuado
												FROM
													academico.cursoequivalencia
												$subWhere)");
		}

		$cdtcod = $_SESSION["academico"]["tipocurso"] == TIPOCURSOGRADUACAO ? "cdtcodigoemec" : "cdtcodcapes";

		$campoTurno = $param['tipoLista'] == 'P' ? "at.turdsc" : "at2.turdsc";
		$campoAnoIni = $param['tipoLista'] == 'P' ? "cd.cdtinicioprev" : "cd.cdtinicioexec";

		if ( count( $grupoEquiv ) ){
			$in = implode(" , ", $grupoEquiv);
			$op = <<<EOT
			'<input id="$nome' || c.curid || '" type="checkbox" ' ||
			CASE WHEN c.curid IN ( $in )
				THEN
					'checked="checked"'
				ELSE
					''
			END
			|| ' name="{$nome}[]" value="' || c.curid || '" onclick="$func"><label for="$nome' || c.curid || '">' || c.curid || ' - ' || c.curdsc || ' (' || {$cdtcod} || ' - ' || e.entnome || ' - ' || {$campoTurno} || ' - ' || {$campoAnoIni} || ' )' || '</label>'
EOT;
		}else{
			$op = <<<EOT
			'<input id="$nome' || c.curid || '" type="checkbox" name="{$nome}[]" value="' || c.curid || '" onclick="$func"><label for="$nome' || c.curid || '">' || c.curid || ' - ' || c.curdsc || ' (' || e.entnome || ' - ' || {$campoTurno} || ' - ' || {$campoAnoIni} || ')' || '</label>'
EOT;
		}
		$sql = "SELECT
					$op AS acao
				FROM
					public.curso c
				INNER JOIN
					academico.cursodetalhe cd ON cd.curid = c.curid
											     AND cd.cdtstatus = 'A'
				LEFT JOIN
					academico.turno at ON at.turid = cd.turidprevisto
				LEFT JOIN
					academico.turno at2 ON at2.turid = cd.turidexecutado
				INNER JOIN
					entidade.entidade e ON e.entid = cd.entid
				WHERE
					c.tpcid = {$_SESSION["academico"]["tipocurso"]} " .
					( $entidcampus ? " AND c.entid in (" . implode( ",", $entidcampus ) . ") AND " : "" ) .
					( implode(" AND ", $where) ) . "
				ORDER BY
					c.curdsc;";

		$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N' );
	}


	function carregaGrupoCursoEquivalencia( $filtro=null, Array $param = null ){
		$where = array();
		$inner = array();

		if ( is_array($filtro) ){
			foreach ($filtro AS $k => $val){
				switch ($k){
					default:
						$where[] = "$k = '$val'";
				}
			}
		}

		if ( $param['tipoEquivalencia'] == 'E' ){
			$cdON = "cd.curid = ce.curidexecutado";
			array_push($where, "cd.cdtpactuacao = 'E'");
		}else{
			$cdON = "cd.curid = ce.curidpactuado";
			array_push($where, "cd.cdtpactuacao = 'P'");
		}

		if ( $param['tipoLista'] == 'text' ){
			$select = "c.curdsc || ' (' || e.entnome || ')'";
			array_push($inner, "INNER JOIN
									public.curso c ON c.curid = cd.curid");
			array_push($inner, "INNER JOIN
									entidade.entidade e ON e.entid = cd.entid");
		}else{
			$select = 'curid';
		}

		$sql = "SELECT
					DISTINCT $select
				FROM
					academico.cursoequivalencia ce
					INNER JOIN
						academico.cursodetalhe cd ON $cdON
					" . ( implode(" ", $inner) ) . "
				WHERE
					" . ( implode(" AND ", $where) );

		$dados = $this->db->carregarColuna( $sql );
		return $dados;
	}

	/*
	 * Função  manterGrupoequivalencia
	 * Método usado para manter (insert/update) os dados da tabela (academico.grupoequivalencia)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    15-12-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterGrupoequivalencia($dados, $where = null, Array $param = null){
		$return   = true;
		$tabela   = "academico.grupoequivalencia";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"greid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"docid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"entid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"gredtinclusao" => array(
							"chave"   => null,
							"value"   => date('d-m-Y'),
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => false,
						),
					"usucpf" => array(
							"chave"   => "FK",
							"value"   => $_SESSION["usucpf"],
							"type"    => "string",
							"tamanho" => "11",
							"mascara" => "cpf",
							"nulo"    => true ,
						)
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
		if ( $param['commit'] ){
			if ($return){
				$this->db->commit();
			}else{
				$this->db->rollback();
			}
		}
		return $return;
	}


	/*
	 * Função  manterCursoequivalencia
	 * Método usado para manter (insert/update) os dados da tabela (academico.cursoequivalencia)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    14-12-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterCursoequivalencia($dados, $where = null){
		$return   = true;
		$tabela   = "academico.cursoequivalencia";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"ceqid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"curidpactuado" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"curidexecutado" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"greid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						)
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		// Caso seja um INSERT popula-se a tabela de grupoequivalencia
		if ( $atributoWhere == null ){
			$dGrupo =  array(
								"entid" => $_SESSION['academico']['entid']
							 );

			$greid = $this->manterGrupoequivalencia( $dGrupo );
			$dados['greid'] = $greid;
		}else{
			// Deleta Curso Equivalência
			$this->db->delete( "academico.cursoequivalencia", array("greid" => $dados['greid']) );
			$greid = $dados['greid'];
			// Zera "atributoWhere", para forçar a inserção dos dados novamente
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					if ( is_array( $val ) && count($val) == 1 ){
						$val = $val[0];
					}
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}


		if ( count($dados['curidpactuado']) > count($dados['curidexecutado']) ){
			$campo  = 'curidpactuado';
//			$campo2 = 'curidexecutado';
			$loop   = count($dados['curidpactuado']);
		}else if ( count($dados['curidpactuado']) < count($dados['curidexecutado']) ){
			$campo  = 'curidexecutado';
//			$campo2 = 'curidpactuado';
			$loop   = count($dados['curidexecutado']);
		}else{
			$campo = '';
			$loop  = 1;
		}
		$cont = 0;

		while ( $loop > $cont && $return ):
			if ( $campo != '' ){
				$atributo->{$campo}['value'] = $dados[$campo][$cont];
			}
			// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
			// retornará FALSE
			// senão o ID do insert


			$return = $this->db->insert($tabela, $atributo, $atributoWhere);

			$cont++;

		endwhile;

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
		if ($return){
			$return = $greid;
			$this->db->commit();
		}else{
			$this->db->rollback();
		}

		return $return;
	}

	function deletarGrupoequivalencia($greid){
		$return = true;

		if ( $greid ){
			$sql = "UPDATE
						academico.cursodetalhe
					SET
						cdtliberdistribuicao = ''
					WHERE
						curid IN (
									SELECT
										DISTINCT c.curid
									FROM
										public.curso c
									INNER JOIN
										academico.cursoequivalencia ce ON ce.curidpactuado = c.curid
														  				  OR ce.curidexecutado = c.curid
									WHERE
										greid = $greid
								  )";
			$this->db->executar( $sql );

			// Deleta Curso Equivalência
			$return = $this->db->delete( "academico.cursoequivalencia", array("greid" => $greid) );
			if ( $return ){
				// Deleta Grupo Curso Equivalência
				$return = $this->db->delete( "academico.grupoequivalencia", array("greid" => $greid) );
			}

			// Verificação do retorno
			if ($return){
				$this->db->commit();
			}else{
				$this->db->rollback();
			}
		}
		return $return;
	}

	function aprovarEquivalencia( $greid ){
		$greid = (integer) $greid;
//		$c = new CursosEdital();

		$dados = array(
						"usucpf" => $_SESSION['usucpf']
					  );
		$where = array(
						"greid" => $greid
					  );
		$this->manterGrupoequivalencia( $dados, $where );

		$sql = "UPDATE
					academico.cursodetalhe
				SET
					cdtliberdistribuicao = 'S'
				WHERE
					curid IN (
								SELECT
									DISTINCT c.curid
								FROM
									public.curso c
								INNER JOIN
									academico.cursoequivalencia ce ON ce.curidpactuado = c.curid
													  				  OR ce.curidexecutado = c.curid
								WHERE
									greid = $greid
							  )";
		$this->db->executar( $sql );
		$this->db->commit();
	}

	function retornarAprovacaoEquivalencia( $greid ){
		$greid = (integer) $greid;
//		$c = new CursosEdital();

		$dados = array(
						"usucpf" => ''
					  );
		$where = array(
						"greid" => $greid
					  );
		$this->manterGrupoequivalencia( $dados, $where );

		$sql = "UPDATE
					academico.cursodetalhe
				SET
					cdtliberdistribuicao = ''
				WHERE
					curid IN (
								SELECT
									DISTINCT c.curid
								FROM
									public.curso c
								INNER JOIN
									academico.cursoequivalencia ce ON ce.curidpactuado = c.curid
													  				  OR ce.curidexecutado = c.curid
								WHERE
									greid = $greid
							  )";
		$this->db->executar( $sql );
		$this->db->commit();
	}

	function exibiDadosEditalCurso( $edtid ){
		$edtid = (integer) $edtid;

		$sql = "SELECT
				    edtdsc, edtnumero, TO_CHAR(edtdtcriacao,'DD/MM/YYYY') AS edtdtcriacao,
				    edtdtpubldiario, edtnumdiario, edtsecaodiario,
				    edtdiariopagina, edtano, TO_CHAR(edtdtinicioinscricao,'DD/MM/YYYY') AS edtdtinicioinscricao,
			        TO_CHAR(edtdtfinalinscricao,'DD/MM/YYYY') AS edtdtfinalinscricao, TO_CHAR(edtdtprovainicio,'DD/MM/YYYY') AS edtdtprovainicio, TO_CHAR(edtdtprovafinal,'DD/MM/YYYY') AS edtdtprovafinal,
			        TO_CHAR(edtdtinicioaulas,'DD/MM/YYYY') AS edtdtinicioaulas, u.usunome, edtnumvagas
				FROM
					academico.editalcurso ec
				INNER JOIN
					seguranca.usuario u ON u.usucpf = ec.usucpf
				WHERE
					ec.edtid = $edtid";
		$dados = $this->db->pegaLinha( $sql );

		$htm = <<<ASDF
			<table width="100%">
				<tr>
					<td><b>Cadastrante:</b></td>
					<td>{$dados['usunome']}</td>
				</tr>
				<tr>
					<td><b>Nº do Edital:</b></td>
					<td>{$dados['edtnumero']}</td>
				</tr>
				<tr>
					<td><b>Nome do Edital:</b></td>
					<td>{$dados['edtdsc']}</td>
				</tr>
				<tr>
					<td><b>Data:</b></td>
					<td>{$dados['edtdtcriacao']}</td>
				</tr>
				<tr>
					<td><b>Total de Vagas:</b></td>
					<td>{$dados['edtnumvagas']}</td>
				</tr>
				<tr>
					<td><b>Ano:</b></td>
					<td>{$dados['edtano']}</td>
				</tr>
				<tr>
					<td><b>Inscrições:</b></td>
					<td>{$dados['edtdtinicioinscricao']} até {$dados['edtdtfinalinscricao']}</td>
				</tr>
				<tr>
					<td><b>Provas:</b></td>
					<td>{$dados['edtdtprovainicio']} até {$dados['edtdtprovafinal']}</td>
				</tr>
				<tr>
					<td><b>Data de Início das Aulas:</b></td>
					<td>{$dados['edtdtinicioaulas']}</td>
				</tr>
				<tr>
					<td><b>Nº do DOU:</b></td>
					<td>{$dados['edtnumdiario']}</td>
				</tr>
				<tr>
					<td><b>Data do DOU:</b></td>
					<td>{$dados['edtdtpubldiario']}</td>
				</tr>
				<tr>
					<td><b>Nº da Seção:</b></td>
					<td>{$dados['edtsecaodiario']}</td>
				</tr>
				<tr>
					<td><b>Nº da Página:</b></td>
					<td>{$dados['edtdiariopagina']}</td>
				</tr>
			</table>
ASDF;
		return $htm;
	}

	function equivalenciaExecutada( $greid ){
		$greid = (int) $greid;
		$sql = "SELECT
					count(ec.excid) as num
				FROM
					academico.grupoequivalencia ge
				INNER JOIN
					academico.cursoequivalencia ce ON ce.greid = ge.greid
				INNER JOIN
					academico.cursodetalhe cd1 ON cd1.curid = ce.curidexecutado
								  			      AND cd1.cdtliberdistribuicao = 'S'
								      			  AND cd1.cdtstatus = 'A'
				INNER JOIN
					academico.execucaocurso ec ON ec.cdtid = cd1.cdtid
								      			  AND ec.excstatus = 'A'
				WHERE
					ge.greid = $greid";
		return $this->db->pegaUm( $sql );
	}
}