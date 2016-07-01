<?php

function monta_lista_simples_div($sql,$cabecalho="",$perpage,$pages,$soma='N',$largura='95%', $valormonetario='S') {
	global $db;
	if(!(bool)$largura) $largura = '95%';
	// este método monta uma listagem na tela baseado na sql passada
	//Registro Atual (instanciado na chamada)
	if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);

    if (is_array($sql))
        $RS = $sql;
    else
        $RS = $db->carregar($sql);

	$nlinhas = $RS ? count($RS) : 0;
	if (! $RS) $nl = 0; else $nl=$nlinhas;
	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
	print '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
	if ($nlinhas>0)
	{
		//Monta Cabeçalho
		if(is_array($cabecalho))
		{
			print '<thead><tr>';
			for ($i=0;$i<count($cabecalho);$i++)
			{
				print '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].'</label>';
			}
			print '</tr> </thead>';
		}

        echo '<tbody>';

		//Monta Listagem
		$totais = array();
		$tipovl = array();
		$x = 0;
		for ($i=($numero-1);$i<$reg_fim;$i++)
		{
			$c = 0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
			foreach($RS[$i] as $k=>$v) {

				if (is_numeric($v))
				{
					//cria o array totalizador
					if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
					//Mostra o resultado
					$id1 = $v;
					if( $k == 'obrid' ){
						unset($v); 
					}
//					unset($v); 
					if (strpos($v,'.')) {$v = simec_number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
					if ($v<0) print '<td align="right" title="'.$cabecalho[$c].'">('.$v.')'; else print '<td align="right" title="'.$cabecalho[$c].'">'.$v;
					print ('<br>'.$totais[$c]);
				}
				else print '<td title="'.$cabecalho[$c].'">'.$v;
				print '</td>';
				$c = $c + 1;
			}
			
			print '</tr>';
			print '<tr id="tr_'.$id1.'" style="display:none;">';
				print '<td colspan="'.$c.'" >';
				print '<div id="div_'.$id1.'" style="width:100%; height:100px; overflow:auto;"></div>';
				print '</td>';
			print '</tr>';
			$x++;
		}

        print '</tbody>';

		$somarCampos = $soma!='S' && is_array($soma) && (@count($soma)>0);
		if ($soma=='S' || $somarCampos){
			//totaliza (imprime totais dos campos numericos)
			print '<tfoot><tr>';
			for ($i=0;$i<$c;$i++)
			{
				print '<td align="right" title="'.$cabecalho[$i].'">';

				if ($i==0) print 'Totais:   ';
				if(($somarCampos && $soma[$i]) || $soma=='S') {
					if (is_numeric($totais['0'.$i])) 
						if($valormonetario == 'S'){
							print simec_number_format($totais['0'.$i], 2, ',', '.'); 
						}else{
							print $totais['0'.$i]; 
						}
					else 
						print $totais['0'.$i];
				}
				print '</td>';
			}
			print '</tr></tfoot>';
			//fim totais
		}

	}
	else {
		print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
	}
	print '</table>';
}

/*********************************************
 *             Funções usadas no             * 
 *              ENTRATO DA OBRA              * 
 *               21-10-2013                  *                                  
 *              SIMEC PATTERN                *
 *********************************************/

/**
 * Retorna tabela com local da obra
 * @global object $db
 * @param int $obrid
 * @return string
 */
function getLocalObra($obrid, $onlyData = false) {
    
    global $db;
    
    $obra = new Obras();
    $contrato = $obra->pegaContratoObra($obrid);
    $andWhere = ($contrato) ? "AND oc.ocrstatus = 'A'" : '';
    
    $strSQLLocalObra = "SELECT ee.endid, ee.endcep, ee.endlog, ee.medlatitude, ee.medlongitude, 
        ee.endnum, ee.endcom, ee.endbai, ee.muncod, ee.estuf, tm.mundescricao, 
        oo.tobid, oo.frpid, oo.iexid, oo.obrstatusinauguracao, oo.entid, oo.obrnome, oo.obrdsc, oo.tpoid, oo.cloid,   
        frr.frrdescvlr, frr.frrdescinstituicao, frr.frrdescnumport, frr.frrdescobjeto,
        c.crtdtassinatura, c.crtprazovigencia, c.crtdttermino, c.entidempresa, 
        oc.umdid, oc.ocrqtdconstrucao, oc.ocrdtordemservico, oc.ocrdtinicioexecucao, oc.ocrprazoexecucao, 
        oc.ocrdtterminoexecucao, oc.ocrvalorexecucao, oc.ocrcustounitario, oc.ocrpercentualdbi, oc.ocrstatus,
        e.empesfera, e.entidunidade, e.empcomposicao, c.crtid
    FROM entidade.endereco ee
        JOIN obras2.obras oo ON (oo.endid = ee.endid)
        JOIN territorios.municipio tm ON (tm.muncod = ee.muncod)
        LEFT JOIN obras2.obrascontrato oc ON (oc.obrid = oo.obrid)
        LEFT JOIN obras2.formarepasserecursos frr ON (frr.obrid = oo.obrid)
        LEFT JOIN obras2.contrato c ON (c.obrid_1 = oo.obrid)
        JOIN obras2.empreendimento e ON (e.empid = oo.empid)
    WHERE oo.obrid = %s {$andWhere}";
    $strSql = sprintf($strSQLLocalObra, (int) $obrid);
    
    $dobras = current($db->carregar($strSql));
    
    if ($onlyData) {
        return $dobras;
    }
    
    return "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>
        <tr>
            <td class='subtitulocentro' colspan='2' height='25px;'>Local da Obra</td>
        </tr>
        <tr>
            <td width='10%' class='subtitulodireita' style='font-weight: bold;'>CEP</td>
            <td bgcolor='#f5f5f5'>{$dobras['endcep']}</td>
        </tr>
        <tr>
            <td class='subtitulodireita' style='font-weight: bold;'>Logradouro</td>
            <td>{$dobras['endlog']}</td>
        </tr>
        <tr>
            <td class='subtitulodireita' style='font-weight: bold;'>Número</td>
            <td bgcolor='#f5f5f5'>" . ($dobras['endnum'] ? $dobras['endnum'] : "Não Informado") . "</td>
        </tr>
        <tr>
            <td class='subtitulodireita' style='font-weight: bold;'>Complemento</td>
            <td>" . ($dobras['endcom'] ? $dobras['endcom'] : "Não Informado") . "</td>
        </tr>
        <tr>
            <td class='subtitulodireita' style='font-weight: bold;'>Bairro</td>
            <td bgcolor='#f5f5f5'>{$dobras['endbai']}</td>
        </tr>
        <tr>
            <td class='subtitulodireita' style='font-weight: bold;'>Município/UF</td>
            <td>{$dobras['mundescricao']} / {$dobras['mundescricao']}</td>
        </tr>
    </table>";
}

/**
 * 
 * @global object $db
 * @param type $obrid
 * @return string
 */
function getContatosObra($obrid, $orgid) {
    global $db;
    
    $sqlUf = "SELECT ee.estuf 
    FROM entidade.endereco ee 
    JOIN obras2.obras oo ON (oo.endid = ee.endid)
    WHERE oo.obrid = {$obrid}";
    $rs = current($db->carregar($sqlUf));
    
    $strSqlContatos = "SELECT DISTINCT 
    REPLACE(TO_CHAR(TRIM(u.usucpf)::numeric, '000:000:000-00'),':', '.') AS cpf,
    u.usunome AS nome, 
    u.usuemail AS email,
    CASE WHEN et.entnumdddcomercial != '' THEN 
        '(' || et.entnumdddcomercial || ') ' || et.entnumcomercial
    WHEN u.usufoneddd != '' THEN 
        '(' || u.usufoneddd || ') ' || COALESCE(u.usufonenum, '')
    ELSE 
        COALESCE(u.usufonenum, '')
    END AS telefone 
    FROM 
        obras2.contato c 
    LEFT JOIN
        seguranca.usuario u ON (u.usucpf = c.usucpf) 
    INNER JOIN 
        entidade.entidade et ON (u.entid = et.entid)
    WHERE 
        c.cntstatus = 'A' AND 
        c.orgid IN({$orgid}) AND 
            c.estuf IN('{$rs['estuf']}')";
    
    $dadosContatos = $db->carregar($strSqlContatos);
    
    $tabelaContatos = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
    
    if ($dadosContatos) {
        
        $tabelaContatos .= "<tr>
            <td class='subtitulocentro'>CPF</td>
            <td class='subtitulocentro'>Nome do Responsável</td>
            <td class='subtitulocentro'>E-mail</td>
            <td class='subtitulocentro'>Telefone</td>
            <!-- <td class='subtitulocentro'>Tipo de Responsabilidade</td> -->
       </tr>";

        for( $i = 0; $i < count($dadosContatos); $i++ ){
            $cor = $i % 2 ? "#e0e0e0" : '#ffffff'; 
            $tabelaContatos .= "<tr bgcolor='{$cor}'>
                <td align='center'>{$dadosContatos[$i]["cpf"]}</td>
                <td align='center'>{$dadosContatos[$i]["nome"]}</td>
                <td align='center'>" . ( $dadosContatos[$i]["email"] ? $dadosContatos[$i]["email"] : "Não informado" ). "</td>
                <td align='center'>{$dadosContatos[$i]["telefone"]}</td>
                <!-- <td align='center'>{$dadosContatos[$i]["tipo_desc"]}</td> -->
            </tr>";
        }

    }else{
        $tabelaContatos .= "<tr>
            <td align='center' style='color:#ee0000'>Não existem contatos cadastrados para a obra.</td>
        </tr>";
    }
    $tabelaContatos .= "</table>";
    
    return "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>
        <tr>
            <td class='subtitulocentro' colspan='4' height='25px;'>Contatos</td>
        </tr>
        <tr>
            <td colspan='2'>$tabelaContatos</td>
        </tr>
    </table>";
}

/**
 * Retorna todos os responsaveis de uma obra
 * @global object $db
 * @param int $obrid
 * @return string
 */
function getResponsaveis($empid) {
    
    global $db;
    
    $strSql = "SELECT 
        REPLACE(TO_CHAR(TRIM(u.usucpf)::numeric, '000:000:000-00'),':', '.') AS cpf,
        u.usunome AS nome, 
        u.usuemail AS email, 
        CASE WHEN u.usufoneddd != '' THEN
            '(' || u.usufoneddd || ') ' || COALESCE(u.usufonenum, '')
        ELSE
            COALESCE(u.usufonenum, '')
        END AS telefone, 
        array_to_string(array(select pfldsc from seguranca.perfil where pflcod = ur.pflcod and pflstatus = 'A'),', br') AS usuperfil
    FROM obras2.usuarioresponsabilidade ur
        INNER JOIN seguranca.usuario u on u.usucpf = ur.usucpf
    WHERE 
        ur.empid = {$empid}
        and ur.rpustatus = 'A'";
    $dadosResponsaveis = $db->carregar($strSql);
			
    if ($dadosResponsaveis) {
        $tabelaResponsaveis = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
        $tabelaResponsaveis .= "<tr>
            <td class='subtitulocentro'>CPF</td>
            <td class='subtitulocentro'>Nome do Responsável</td>
            <td class='subtitulocentro'>E-mail</td>
            <td class='subtitulocentro'>Telefone</td>
            <td class='subtitulocentro'>Perfil</td>
      </tr>";

        for( $i = 0; $i < count($dadosResponsaveis); $i++ ) {

            $cor = $i % 2 ? "#e0e0e0" : '#ffffff'; 

            $tabelaResponsaveis .= "<tr bgcolor='{$cor}'>
                <td align='center'>{$dadosResponsaveis[$i]["cpf"]}</td>
                <td align='center'>{$dadosResponsaveis[$i]["nome"]}</td>
                <td align='center'>" . ( $dadosResponsaveis[$i]["email"] ? $dadosResponsaveis[$i]["email"] : "Não informado" ). "</td>
                <td align='center'>{$dadosResponsaveis[$i]["telefone"]}</td>
                <td align='center'>{$dadosResponsaveis[$i]["usuperfil"]}</td>
          </tr>";
        }
    }else{
        $tabelaResponsaveis .= "<tr>
            <td align='center' colspan='4' style='color:#ee0000'>Não existem responsáveis cadastrados para a obra.</td>
        </tr>";
    }
    $tabelaResponsaveis .= "</table>";
    
    return "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>
        <tr>
            <td class='subtitulocentro' colspan='2' height='25px;'>Responsáveis</td>
        </tr>
        <tr>
            <td colspan='2'>$tabelaResponsaveis</td>
        </tr>
    </table>";
}

/**
 * 
 * @global object $db
 * @param int $tobid
 * @return string
 */
function getTipoObra($tobid) {
    global $db;
    $strSQL = 'SELECT tobdesc FROM obras2.tipoobra WHERE tobid = %s';
    $stmt = sprintf($strSQL, (int) $tobid);
    return $db->pegaUm($stmt) ? $db->pegaUm($stmt) : 'Não informado';
}

/**
 * 
 * @global object $db
 * @param int $entid
 * @return array
 */
function getInfoEntEmpresa($obrID) {
    
    $obra     = new Obras( $obrID );
    $crtid    = $obra->pegaContratoPorObra( $obrID );
    $contrato = new Contrato( $crtid );
    $dados    = $contrato->getDados();
    $output   = $obra->getDadosCabecalho($obrID);
    
    foreach ($dados as $k => $v) {
        $output[$k] = trim($v);
    }
    
    $empresa = new Entidade($output['entidempresa']);
    $output['nome']    = trim($empresa->entnome);
    $output['email']   = trim($empresa->entemail);
    $output['cpfcnpj'] = trim($empresa->entnumcpfcnpj);
    $output['ddd']     = trim($empresa->entnumdddcomercial);
    $output['numero']  = trim($empresa->entnumcomercial);
    $output['entrazaosocial'] = trim($empresa->entrazaosocial);
    $output['entid']   = $empresa->entid;
    
    $obraContrato = new ObrasContrato();
    $dadosObraContrato = current($obraContrato->listaByContrato( $crtid, $obrID ));
    $output['ocrdtordemservico']    = formata_data($dadosObraContrato['ocrdtordemservico']);
    $output['ocrdtinicioexecucao']  = formata_data($dadosObraContrato['ocrdtinicioexecucao']);
    $output['ocrdtterminoexecucao'] = formata_data($dadosObraContrato['ocrdtterminoexecucao']);
    $output['ocrvalorexecucao']     = simec_number_format($dadosObraContrato['ocrvalorexecucao'], 2, ',', '.');
    $output['ocrcustounitario']     = simec_number_format($dadosObraContrato['ocrcustounitario'], 2, ',', '.');
    $output['ocrpercentualdbi']     = simec_number_format($dadosObraContrato['ocrpercentualdbi'], 2, ',', '.');
    $output['ocrqtdconstrucao']     = simec_number_format($dadosObraContrato['ocrqtdconstrucao'], 2, ',', '');
    
    return $output;
}

/**
 * 
 * @global object $db
 * @param int $obrID
 * @return string
 */
function getSituacaoObra($obrID) {
    global $db;
    $strSQL = "SELECT s.sobdsc
    FROM obras2.obrainfraestrutura o
    INNER JOIN obras2.situacaoobra s ON (s.sobid = o.sobid)
    WHERE o.obrid = %s";
    $stmt = sprintf($strSQL, (int) $obrID);
    return $db->pegaUm($stmt) ? $db->pegaUm($stmt) : 'Não informado';
}

/**
 * 
 * @param string $obrstatusinauguracao
 * @return string
 */
function getStatusInauguracao($obrstatusinauguracao) {
    switch ($obrstatusinauguracao) {
        case 'S': $inContratacao = 'Não se Aplica';
        break;
        case 'N': $inContratacao = 'Não Inaugurada';
        break;
        case 'I': $inContratacao = 'Inaugurada';
        break;
        default: $inContratacao = 'Não informado';
    }
    return $inContratacao;
}

/**
 * 
 * @global object $db
 * @param int $frpid
 * @return string
 */
function getTipoRepasseRecurso($frpid) {
    global $db;
    $strSQL = "SELECT frpdesc AS descricao 
            FROM obras2.tipoformarepasserecursos
            WHERE frpid = %s";
    $stmt = sprintf($strSQL, (int) $frpid);
    return $db->pegaUm($stmt) ? $db->pegaUm($stmt): 'Não informado';
}

/**
 * 
 * @global object $db
 * @param int $obrID
 * @return string
 */
function getDadosConvenio($obrID) {
    global $db;
    
    $strSQL = "SELECT b.*
    FROM obras2.obrainfraestrutura a
    INNER JOIN painel.dadosconvenios b ON (b.dcoprocesso = Replace(Replace(Replace(a.obrnumprocessoconv,'.',''),'/',''),'-',''))
    WHERE obrid = %s";
    $stmt = sprintf($strSQL, (int) $obrID);
    return $db->pegaLinha($stmt);
}

/**
 * 
 * @global object $db
 * @param array $dados_convenio
 * @return string
 */
function getDadosAditivo(array $dados_convenio) {
    
    global $db;
    
    $strSQL = "SELECT 
        substring(dfidatasaldo::varchar,6,2) || '/' || substring(dfidatasaldo::varchar,0,5) AS dfidatasaldo, 
        SUM(dfisaldoconta) as dfisaldoconta, 
        (SUM(dfisaldofundo) + SUM(dfisaldopoupanca) + SUM(dfisaldordbcdb)) AS totalaplicacao, 
        (SUM(dfisaldoconta) + SUM(dfisaldofundo) + SUM(dfisaldopoupanca) + SUM(dfisaldordbcdb)) AS totalconta
    FROM
        painel.dadosfinanceirosconvenios dfi
    WHERE 
        dfiprocesso = '{$dados_convenio['dcoprocesso']}'
    GROUP BY 
        dfidatasaldo
    ORDER BY 
        dfidatasaldo";

    $rsSaldoBancarios = $db->carregar($strSQL);					

    if($rsSaldoBancarios){
        
        $htmlSaldoBancarios = '<table class="tabela" cellSpacing="1" cellPadding="3" align="center" width="95%">
        <thead>
            <th>Mês Ref</th>
            <th>Saldo Conta</th>
            <th>Saldo Aplicações</th>
            <th>Total Conta</th>
        </thead>
       <tbody>';
        
        foreach($rsSaldoBancarios as $saldoBancario){

            $htmlSaldoBancarios .= "<tr>
                <td>{$saldoBancario['dfidatasaldo']}</td>
                <td>{$saldoBancario['dfisaldoconta']}</td>
                <td>{$saldoBancario['totalaplicacao']}</td>
                <td>{$saldoBancario['totalconta']}</td>
            </tr>";
        }
        $htmlSaldoBancarios .= '</tbody></table>';
    }else{
        $htmlSaldoBancarios = 'Sem registros.';
    }
    
    return $htmlSaldoBancarios;
}

/**
 * 
 * @global object $db
 * @param array $dados_convenio
 * @return string
 */
function getAditivosRepasse(array $dados_convenio) {
    
    global $db;
    
    $strSQL = "SELECT
                TO_CHAR(drcdatapagamento, 'DD/MM/YYYY') AS datapagamento, 
                drcordembancaria, 
                drcvalorpago, 
                'Banco: ' || drcbanco || '<br>Ag: ' || drcagencia || '<br>CC: ' || drcconta AS drcdadosbancarios
            FROM
              painel.dadosrepassesconvenios drc
            WHERE
              drcprocesso = '{$dados_convenio['dcoprocesso']}'
            ORDER BY
              drcdatapagamento";
    $rsRepasse = $db->carregar($strSQL);

    if ($rsRepasse) {

        $htmlRepasse = '<table class="tabela" cellSpacing="1" cellPadding="3" align="center" width="95%">
                            <thead>
                                <th>Data do Repasse</th>
                                <th>OB</th>
                                <th>Valor Repassado</th>
                                <th>Dados Bancários</th>
                            </thead>
                        <tbody>';

        foreach ($rsRepasse as $repasse) {

            $htmlRepasse .= "<tr>
                                <td>{$repasse['datapagamento']}</td>
                                <td>{$repasse['drcordembancaria']}</td>
                                <td>{$repasse['drcvalorpago']}</td>
                                <td>{$repasse['drcdadosbancarios']}</td>
                             </tr>";
        }
        $htmlRepasse .= '</tbody></table>';
    } else {
        $htmlRepasse = 'Sem registros.';
    }
    
    return $htmlRepasse;
}

/**
 * 
 * @param type $dados_convenio
 * @return string
 */
function recursosPorConvenio($dados_convenio) {
    
    $dadosDetalheForma = "<tr><td colspan='2'>"
    . "<table class='tabela' cellSpacing='1' cellPadding='3' align='center' width='95%'>"
    . "    <tr>"
    . "        <td class='subtitulodireita' width='180'>Número do Convênio</td>"
    . "        <td bgcolor='#f5f5f5'>{$dados_convenio['dcoconvenio']}</td>"
    . "    </tr>"
    . "    <tr>"
    . "        <td class='subtitulodireita'>Ano</td>"
    . "        <td>".formata_data($dados_convenio['dcoano'])."</td>"
    . "    </tr>"
    /*
    . "    <tr>"
    . "        <td class='subtitulodireita'>Objeto</td>"
    . "        <td bgcolor='#f5f5f5'>".$dados_convenio["covobjeto"]."</td>"
    . "    </tr>"
    . "    <tr>"
    . "        <td class='subtitulodireita'>Detalhamento</td>"
    . "        <td>".$dados_convenio["covdetalhamento"]."</td>"
    . "    </tr>"
    */
    . "    <tr>"
    . "        <td class='subtitulodireita'>Processo</td>"
    . "        <td bgcolor='#f5f5f5'>".$dados_convenio['dcoprocesso']."</td>"
    . "    </tr>"
    . "    <tr>"
    . "        <td class='subtitulodireita'>Concedente</td>"
    . "        <td>".simec_number_format($dados_convenio['dcovalorconcedente'],2,',','.')."</td>"
    . "    </tr>"
    . "    <tr>"
    . "        <td class='subtitulodireita'>Convenente</td>"
    . "        <td bgcolor='#f5f5f5'>".simec_number_format($dados_convenio['dcovalorcontrapartida'],2,',','.')."</td>"
    . "    </tr>"
    . "    <tr>"
    . "        <td class='subtitulodireita'>Valor (R$)</td>"
    . "        <td>".simec_number_format($dados_convenio['dcovalorcontrapartida']+$dados_convenio['dcovalorconcedente'],2,',','.')."</td>"
    . "    </tr>"
    . "    <tr>"
    . "        <td class='subtitulodireita'>Início</td>"
    . "        <td bgcolor='#f5f5f5'>".formata_data($dados_convenio['dcodatainicio'])."</td>"
    . "    </tr>"
    . "    <tr>"
    . "        <td class='subtitulodireita'>Fim</td>"
    . "        <td>".formata_data($dados_convenio['dcodatafim'])."</td>"
    . "    </tr>"

    . "    <tr>"
    . "        <td colspan='2'>Saldos Bancários</td>"										
    . "    </tr>"
    . "    <tr>"
    . "        <td colspan='2'>".getDadosAditivo($dados_convenio)."</td>"										
    . "    </tr>"
    . "    <tr>"
    . "        <td colspan='2'>Repasse</td>"										
    . "    </tr>"
    . "    <tr>"
    . "        <td colspan='2'>".getAditivosRepasse($dados_convenio)."</td>"										
    . "    </tr>"
    . "</table>"
    . "</tr></td>";
    
    return $dadosDetalheForma;
}

/**
 * 
 * @param array $dadosObra
 * @return type
 */
function recursosPorDescentralizacao(array $dadosObra) {
    
    $frrdescvlr = simec_number_format($dadosObra['frrdescvlr'], 2, ',', '.' );
    
    $dadosDetalheForma = "<tr><td>
    <table class='tabela' cellSpacing='1' cellPadding='3' align='center'>
       <tr>
           <td class='subtitulodireita' style='font-weight: bold;'>Instituição</td>
           <td bgcolor='#f5f5f5'>{$dadosObra['frrdescinstituicao']}</td>
       </tr>
       <tr>
           <td class='subtitulodireita' style='font-weight: bold;'>Número da Portaria de Descentralização</td>
           <td bgcolor='#f5f5f5'>{$dadosObra['frrdescnumport']}</td>
       </tr>
       <tr>
           <td class='subtitulodireita' style='font-weight: bold;'>Objeto</td>
           <td bgcolor='#f5f5f5'>{$dadosObra['frrdescobjeto']}</td>
       </tr>
       <tr>
           <td class='subtitulodireita' style='font-weight: bold;'>Valor (R$)</td>
           <td bgcolor='#f5f5f5'>{$frrdescvlr}</td>
       </tr>
    </table>
    </tr></td>";
    
    return $dadosDetalheForma;
}

/**
 * 
 * @global object $db
 * @param int $umdid
 * @return string
 */
function getUnidadeMedida($umdid) {
    global $db;
    $strSQL = 'SELECT umdeesc AS descricao 
            FROM obras2.unidademedida
            WHERE umdid = %s';
    $stmt = sprintf($strSQL, (int) $umdid);
    return $db->pegaUm($stmt) ? $db->pegaUm($stmt) : 'Não informado';
}

/**
 * 
 * @param array $dadosObra
 * @return string
 */
function recursoDefault(array $dadosObra) {
    $dadosDetalheForma =  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Observação</td>"
    .  "    <td bgcolor='#f5f5f5'>" . ($dadosObra['frrobsrecproprio'] ? $dadosObra['frrobsrecproprio'] : 'Não informado') . "</td>"
    .  "</tr>";
    
    return $dadosDetalheForma;
}

/**
 * Monta a saída da etapa de contratacoes do relatório
 * @param array $collection
 * @return string
 */
function outPutContratacao(array $collection, $dadosObra) {

    $dadosExtratoObras .= "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>"
    .  "<tr>"
    .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Contratação</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td colspan='2' style='font-weight: bold;'>Contratação da Obra</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Empresa Contratada</td>"
    .  "    <td bgcolor='#f5f5f5'>{$collection['entEmpresa']['nome']}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>E-mail</td>"
    .  "    <td>{$collection['entEmpresa']['email']}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Endereço</td>"
    .  "    <td bgcolor='#f5f5f5'>{$collection['entEmpresa']['endereco']}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Telefone</td>"
    .  "    <td>({$collection['entEmpresa']['ddd']}) {$collection['entEmpresa']['numero']}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Natureza Jurídica</td>"
    .  "    <td bgcolor='#f5f5f5'>{$collection['entEmpresa']['natureza']}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Data de Assinatura do Contrato</td>"
    .  "    <td>" . ( $collection['entEmpresa']['crtdtassinatura'] ? formata_data( $collection['entEmpresa']['crtdtassinatura'] ) : 'Não informado') . "</td>"
    .  "</tr>"


    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Prazo de Vigência do Contrato (dias)</td>"
    .  "    <td>" . ( $collection['entEmpresa']['crtprazovigencia'] ? $collection['entEmpresa']['crtprazovigencia'] : 'Não informado') . "</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Data de término do contrato</td>"
    .  "    <td>" . ( $collection['entEmpresa']['crtdttermino'] ? formata_data( $collection['entEmpresa']['crtdttermino'] ) : 'Não informado') . "</td>"
    .  "</tr>"


    .  "<tr>"
    .  "    <td colspan='2' style='font-weight: bold;'>Sobre a Obra</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Situação da Obra</td>"
    .  "    <td bgcolor='#f5f5f5'>{$collection['stContratacao']['stContratacao']}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Data da Ordem de Serviço</td>"
    .  "    <td>" . ( $collection['entEmpresa']['ocrdtordemservico'] ? $collection['entEmpresa']['ocrdtordemservico'] : 'Não informado') . "</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Início de Execução da Obra</td>"
    .  "    <td bgcolor='#f5f5f5'>" . ( $collection['entEmpresa']['ocrdtinicioexecucao'] ?  $collection['entEmpresa']['ocrdtinicioexecucao'] : 'Não informado') . "</td>"
    .  "</tr>"


    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Prazo de Execução (dias)</td>"
    .  "    <td bgcolor='#f5f5f5'>" . ( $collection['entEmpresa']['ocrprazoexecucao'] ? $collection['entEmpresa']['ocrprazoexecucao'] : 'Não informado') . "</td>"
    .  "</tr>"


    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Término de Execução da Obra</td>"
    .  "    <td>" . ( $collection['entEmpresa']['ocrdtterminoexecucao'] ? $collection['entEmpresa']['ocrdtterminoexecucao']  : 'Não informado') . "</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Valor do Contrato (R$)</td>"
    .  "    <td bgcolor='#f5f5f5'>" . simec_number_format( $collection['entEmpresa']['crtvalorexecucao'], 2, ',' , '.') . "</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Valor Previsto da Obra (R$)</td>"
    .  "    <td bgcolor='#f5f5f5'>" . simec_number_format( $dadosObra['obrvalorprevisto'], 2, ',' , '.') . "</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Valor Contratado (R$)</td>"
    .  "    <td bgcolor='#f5f5f5'>" . simec_number_format( $collection['obrascontrato']['ocrvalorexecucao'], 2, ',' , '.') . "</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Área/Quantidade a ser Construída</td>"
    .  "    <td>" . $collection['entEmpresa']['ocrqtdconstrucao'] . " " . $collection['umContratacao'] . "</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Custo Unitário (R$)</td>"
    .  "    <td bgcolor='#f5f5f5'>" . $collection['entEmpresa']['ocrcustounitario'] . " (R$ / Unidade de Medida)</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Percentual BDI</td>"
    .  "    <td>" . $collection['entEmpresa']['ocrpercentualdbi'] . "</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Tipo de Obra</td>"
    .  "    <td bgcolor='#f5f5f5'>{$collection['tpContratacao']}</td>"
    .  "</tr>"

    /*
    .  "<tr>"
    .  "    <td colspan='2' style='font-weight: bold;'>Situação do Imóvel</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Tipo de Aquisição do Terreno</td>"
    .  "    <td bgcolor='#f5f5f5'>{$aqidsc}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Situação Dominial já Regularizada?</td>"
    .  "    <td bgcolor='#f5f5f5'>" . $iexsitdominialimovelregulariza ? 'Sim' : 'Não' ) . "</td>"
    .  "</tr>"
    */
    
    .  "<tr>"
    .  "    <td colspan='2' style='font-weight: bold;'>Sobre a Inauguração</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Inaugurada</td>"
    .  "    <td bgcolor='#f5f5f5'>{$collection['inContratacao']}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Data da Inauguração</td>"
    .  "    <td>" .  ( $collection['obrdtinauguracao'] ? formata_data( $collection['obrdtinauguracao'] ) : 'Não informado')  . "</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Data de Previsão da Inauguração</td>"
    .  "    <td bgcolor='#f5f5f5'>" . ( $collection['obrdtprevinauguracao'] ? formata_data( $collection['obrdtprevinauguracao'] ) : 'Não informado') . "</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td colspan='2' style='font-weight: bold;'>Forma de Repasse de Recursos</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Tipo</td>"
    .  "    <td bgcolor='#f5f5f5'>{$collection['tfrContratacao']}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td colspan='2' style='font-weight: bold;'>Detalhes</td>"
    .  "</tr>"
    .  $collection['convenio']
    . "</table>";
    
    return $dadosExtratoObras;
}

/**
 * Monta a partes dos dados de contratação com funcoes auxiliares
 * @param array $dadosObra
 * @param int $obrID
 * @return string HTML
 */
function getContratacao($dadosObra, $obrID) {
    $output = array();
    $obra = new Obras($obrID);

    $output['tpContratacao']  = getTipoObra($dadosObra['tobid']);
    $output['entEmpresa']     = getInfoEntEmpresa($obrID);
    
    $output['stContratacao']  = array(); //getSituacaoObra($obrID);
    $output['umContratacao']  = getUnidadeMedida($dadosObra['umdid']);
    $output['inContratacao']  = getStatusInauguracao($dadosObra['obrstatusinauguracao']);
    $output['tfrContratacao'] = getTipoRepasseRecurso($dadosObra['frpid']);

    $obraContrato = new ObrasContrato();
    $dadosObraContrato = $obraContrato->listaByContrato( $dadosObra['crtid'], $obrID );
    $output['obrascontrato'] = $dadosObraContrato[0];

    switch ($dadosObra['frpid']) {
        case 1:
            $dados_convenio = getDadosConvenio($obrID);
            $output['convenio'] = recursosPorConvenio($dados_convenio);
        break;
        
        case 2:
            $output['convenio'] = recursosPorDescentralizacao($dadosObra);
        break;
        
        default:
            $output['convenio'] = recursoDefault($dadosObra);
    }

    return outPutContratacao($output, $obra->getDados());
}

/**
 * 
 * @global object $db
 * @param int $obrID
 * @return array
 */
function getEtapasObra($obrID) {
    
    global $db;
    
    $strSQL = "SELECT 
        i.itcid, i.icovlritem, i.icopercsobreestrutura, i.icopercprojperiodo, i.icopercexecutado, 
        ic.itcdesc, ic.itcdescservico
    FROM 
        obras2.itenscomposicaoobra i
    INNER JOIN
        obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus = 'A'
    INNER JOIN 
        obras2.itenscomposicao ic ON (i.itcid = ic.itcid)
    WHERE
        cro.obrid = %d AND
        i.obrid = cro.obrid AND
        i.icostatus = 'A' 
    ORDER BY 
        i.icoordem";
    
    $stmt = sprintf($strSQL, (int) $obrID);
    return $db->carregar($stmt);
}

/**
 * 
 * @param array $collection
 * @return String HTML
 */
function mountTableEtapasObra($collection) {
    
    $tabelaEtapas = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
    if ($collection) {
        $tabelaEtapas = "<tr>
            <td class='subtitulocentro'>Descrição</td>
            <td class='subtitulocentro'>Valor do Item</td>
            <td class='subtitulocentro'>% Referente a Obra</td>
        </tr>";

        for($i = 0; $i < count($collection); $i++) {
            //$itcid 	      = $collection[$i]['itcid'];
            $icovlritem       = $collection[$i]['icovlritem'];
            $itcdesc 	      = $collection[$i]['itcdesc'];
            $icopercsobreobra = $collection[$i]['icopercsobreobra'];
            //$icopercexecutado = $collection[$i]['icopercexecutado'];
            //$itcdescservico   = $collection[$i]['itcdescservico'];

            $somav 	= bcadd($somav, $icovlritem, 2);
            $icovlritem = simec_number_format($icovlritem,2,',','.'); 
            $soma 	= round($soma, 2) + round($icopercsobreobra, 2);

            $icopercsobreobra = simec_number_format($icopercsobreobra, 2);
            $icopercsobreobra = str_replace('.', ',', $icopercsobreobra);

            $cor = ($i % 2) ? "#e0e0e0" : '#ffffff';

            $tabelaEtapas .= "<tr bgcolor='{$cor}'>
                <td>{$itcdesc}</td>
                <td align='right'>{$icovlritem}</td>
                <td align='right'>{$icopercsobreobra}</td>
            </tr>";
        }
    }else{
        $tabelaEtapas .= "<tr>
            <td align='center' style='color:#ee0000'>Não existem etapas cadastradas para a obra.</td>
         </tr>";
    }
    
    $soma = ($soma > 100.00) ? 100.00 : $soma;
    
    $tabelaEtapas .= "<tr bgcolor='#C0C0C0'>
            <td align='right'><b>Total</b></td>
            <td align=right><b>" . simec_number_format($somav, 2, ',', '.') . "</b></td>
            <td align=right><b>" . simec_number_format($soma, 2, ',', '.') . "</b></td>
          </tr>
      </table>";
    
    return "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>
        <tr>
            <td class='subtitulocentro' colspan='3' height='25px;'>Cronograma Físico-Financeiro</td>
        </tr>
        <tr>
            <td colspan='3'>$tabelaEtapas</td>
        </tr>
    </table>";
}

/**
 * 
 * @param int $obrID
 * @return string HTML
 */
function getLicitacao($obrID) {
    
    $licitacao = new Licitacao();
    $dados = $licitacao->pegaDadosPorObra($obrID);
    $tabelaFlCont = '';
    
    if($dados['licid']){
						
        $faseLic = new FaseLicitacao();
        $arDados = $faseLic->listaPorLicitacao( $dados['licid'] );
        $i = 0;
        
        $tabelaFlCont .= '<table class=\'tabela\' cellSpacing=\'1\' cellPadding=\'3\' align=\'center\'>
        <tr>
            <td class=\'subtitulocentro\'>Descrição</td>
            <td class=\'subtitulocentro\'>Data</td>
        </tr>';
        
        foreach ($arDados as $row) {
            
            switch ($row['tflid']) {
                case 2:
                    $flcdata = formata_data($row['flcpubleditaldtprev']);
                break;
                case 5:
                    $flcdata = formata_data($row['flcdtrecintermotivo']);
                break;
                case 6:
                    $flcdata = formata_data($row['flcordservdt']);
                break;
                case 9:
                    $flcdata = formata_data($row['flchomlicdtprev']);
                break;
                case 7:
                    $flcdata = formata_data($row['flcaberpropdtprev']);
                break;
            }

            $cor = ($i % 2) ? "#e0e0e0" : '#ffffff'; 
            
            $tabelaFlCont .= "<tr bgcolor='{$cor}'>
                <td>
                    <img src='/imagens/consultar.gif' border='0' style='cursor:pointer;' title='Visualizar dados da fase'>
                    &nbsp;{$row['tfldesc']}
                </td>
                <td>{$flcdata}</td>
            </tr>";
            
            $i++;
        }
    } else {
        $tabelaFlCont .= '<tr>
            <td align=\'center\' style=\'color:#ee0000\'>Não existem fases de licitação cadastradas para a obra.</td>
        </tr>';
    }
    
    $tabelaFlCont .= '</table>';
    
    return "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>"
    .  "<tr>"
    .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Licitação</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Modalidade de Licitação:</td>"
    .  "    <td bgcolor='#f5f5f5'>{$dados['moldsc']}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td class='subtitulodireita' style='font-weight: bold;'>Número da Licitação:</td>"
    .  "    <td bgcolor='#f5f5f5'>{$dados['licnumero']}</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td style='font-weight: bold;'>Fases de Licitação</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td colspan='2'>"
    .           $tabelaFlCont
    .  "    </td>"
    .  "</tr>"
    .  "</table>";
}

/**
 * 
 * @global object $db
 * @param type $obrID
 * @return type
 */
function getModelProjetos($obrID) {
    global $db;
    
    $strSQL = "SELECT
        r.rstid,
        CASE WHEN r.rstitem = 'I' THEN 'Inconformidade' ELSE 'Restrição' END as item,
        CASE WHEN fr.fsrid IS NOT NULL THEN fr.fsrdsc ELSE 'Não Informada' END AS fase,
        TO_CHAR(r.rstdtinclusao,'DD/MM/YYYY') as datainclusao,
        r.rstdsc,
        tr.tprdsc,
        r.rstdscprovidencia,
        TO_CHAR(r.rstdtprevisaoregularizacao,'DD/MM/YYYY') AS rstdtprevisaoregularizacao,
        CASE WHEN r.rstsituacao = true THEN TO_CHAR(r.rstdtsuperacao,'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao,
        usu.usucpf AS cpfcriador,
        usu.usunome AS criadopor,
        sup.usunome AS ususuperacao
    FROM
        obras2.restricao r
    INNER JOIN 
        obras2.tiporestricao tr ON (tr.tprid = r.tprid)
    LEFT JOIN
        obras2.faserestricao fr ON (fr.fsrid = r.fsrid)
    INNER JOIN
        seguranca.usuario usu USING (usucpf)
    LEFT JOIN
        seguranca.usuario sup ON (r.usucpfsuperacao = sup.usucpf)
    WHERE
        r.rststatus = 'A' AND
        r.obrid = %d";
    
    $stmt = sprintf($strSQL, (int) $obrID);
    return $db->carregar($stmt);
}

/**
 * 
 * @param type $obrID
 * @return type
 */
function getProjetos($obrID) {
    
    $rsRestricaoProvidencia = getModelProjetos($obrID);
    
    if ($rsRestricaoProvidencia) {

        $dadosExtratoObras = '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
            <thead>
                <tr style="background-color: #e0e0e0">													
                    <td style="font-weight:bold; text-align:center; width:5%">Item</td>
                    <td style="font-weight:bold; text-align:center; width:5%">Fase</td>
                    <td style="font-weight:bold; text-align:center; width:5%">Tipo</td>
                    <td style="font-weight:bold; text-align:center; width:2%">Data da Inclusão</td>
                    <td style="font-weight:bold; text-align:center; width:25%">Descrição</td>
                    <td style="font-weight:bold; text-align:center; width:25%">Providência</td>
                    <td style="font-weight:bold; text-align:center; width:2%">Previsão da Providência</td>
                    <td style="font-weight:bold; text-align:center; width:2%">Superação</td>
                    <td style="font-weight:bold; text-align:center; width:10%">Criado por</td>
                    <td style="font-weight:bold; text-align:center; width:10%">Superado por</td>
                </tr>
            </thead>
        <tbody>';

        foreach ($rsRestricaoProvidencia as $providencia) {

            $dadosExtratoObras .= '<tr>								
                <td align="center">'.$providencia['item'].'</td>
                <td align="center">'.$providencia['fase'].'</td>
                <td align="center">'.$providencia['tprdsc'].'</td>
                <td align="center">'.$providencia['datainclusao'].'</td>
                <td>'.$providencia['rstdsc'].'</td>
                <td>'.$providencia['rstdscprovidencia'].'</td>
                <td align="center">'.$providencia['rstdtprevisaoregularizacao'].'</td>
                <td align="center">'.$providencia['rstdtsuperacao'].'</td>
                <td align="center">'.$providencia['criadopor'].'</td>
                <td align="center">'.$providencia['ususuperacao'].'</td>
              </tr>';
        }

        $dadosExtratoObras .= '</tbody></table>';

    } else {
        $dadosExtratoObras .= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
            <tr>
                <td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
            </tr>
        </table>';
    }

    return "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>"
    .  "<tr>"
    .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Restrições e Providências</td>"
    .  "</tr>"
    .  "<tr>"
    .  "    <td colspan='2'>"
    .           $dadosExtratoObras
    .  "    </td>"
    .  "</tr>"
    .  "</table>";
}

/**
 * 
 * @param array $dadosObra
 * @return string
 */
function getLatLng($dadosObra) {
    
    $longitude = explode('.', $dadosObra['medlongitude']);
    $latitude  = explode('.', $dadosObra['medlatitude']);
    
    $graulongitude = trim($longitude[0]);
    $minlongitude  = trim($longitude[1]);
    $seglongitude  = trim($longitude[2]);
    
    $graulatitude  = trim($latitude[0]);
    $minlatitude   = trim($latitude[1]);
    $seglatitude   = trim($latitude[2]);
    $pololatitude  = trim($latitude[3]);

    //Latitude
    $dadosLatitude = sprintf("%s° %s' %s'' %s", $graulatitude, $minlatitude, $seglatitude, $pololatitude);   
    //Longitude
    $dadosLongitude = sprintf("%s° %s' %s''", $graulongitude, $minlongitude, $seglongitude); 

    return "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>
        <tr>
            <td class='subtitulocentro' colspan='2' height='25px;'>Coordenadas Geográficas</td>
        </tr>
        <tr>
            <td class='subtitulodireita' style='font-weight: bold;'>Latitude</td>
            <td bgcolor='#f5f5f5'>{$dadosLatitude}</td>
        </tr>
        <tr>
            <td class='subtitulodireita' style='font-weight: bold;'>Longitude</td>
            <td>{$dadosLongitude}</td>
        </tr>
    </table>";   
}

/**
 * monta mapa no relatorio
 * @param array $dadosObra
 * @return string
 */
function getMapa($dadosObra) {
    
    $longitude = explode('.', $dadosObra['medlongitude']);
    $latitude  = explode('.', $dadosObra['medlatitude']);
    
    $graulongitude = trim($longitude[0]);
    $minlongitude  = trim($longitude[1]);
    $seglongitude  = trim($longitude[2]);
    
    $graulatitude  = trim($latitude[0]);
    $minlatitude   = trim($latitude[1]);
    $seglatitude   = trim($latitude[2]);
    $pololatitude  = trim($latitude[3]);
    
    $convrLatitude  = ($minlatitude/60) + ($seglatitude/3600);
    $convrLongitude = ($minlongitude/60) + ($seglongitude/3600);
    
    $firstPartLat = explode('.', $convrLatitude);
    $firstPartLng = explode('.', $convrLongitude);
   
    $finalPartLat = substr($firstPartLat[1],-2);
    $finalPartLng = substr($firstPartLng[1],-2);
    
    $secondPartLat = substr($firstPartLat[1],0,4);
    $secondPartLng = substr($firstPartLng[1],0,4);
    
    $latitude  = $graulatitude  . '.' . $secondPartLat.$finalPartLat;
    $longitude = $graulongitude . '.' . $secondPartLng.$finalPartLng;
    
    $gridmapa = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";

    if ($latitude && $longitude) {

        $posicao = ($pololatitude == 'N') ? '' : '-';

        $gridmapa .= "<tr>
            <td colspan='2' align='center'>
                <iframe
                width='400' height='400' frameborder='0' scrolling='no' marginheight='0' marginwidth='0' 
                src='http://maps.google.com/?ie=UTF8&amp;t=k&amp;ll={$posicao}{$latitude},-{$longitude}&amp;spn=0.009417,0.013304&amp;z=16&amp;output=embed&amp;s=AARTsJqzARj-Z8VnW5pkPMLMmZbqrJcYpw'>
                </iframe>
            </td>
            </tr>";

    }else{
        $gridmapa .= "<tr><td colspan='2' align='center'>Dados de localização não cadastrados.</td></tr>";
    }
    
    $gridmapa .= '<table>';
    
    return $gridmapa;   
}


/**
 * 
 * @global object $db
 * @param string $fotosSelecionadas
 * @return type
 */
function getFotos($fotosSelecionadas) {
    global $db;
    
    $strSQL = "SELECT 
        arq.arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao 
    FROM public.arquivo arq
    WHERE arq.arqid IN(%s) AND
        (arq.arqtipo = 'image/jpeg' OR 
         arq.arqtipo = 'image/gif' OR 
         arq.arqtipo = 'image/png') 
    ORDER BY arq.arqid";
    
    $stmt = sprintf($strSQL, $fotosSelecionadas);
    $fotos = $db->carregar($stmt);
    
    if ($fotos) {
		
        $tabelaFotos = '<table class="tabela" cellSpacing="1" cellPadding="3" align="center"><tr>';
        $countFotos = count($fotos);
        $t=0;

        for ($i=0; $i<$countFotos ; $i++ ) {

            if (array_key_exists('arqid', $fotos[$i]) && !empty($fotos[$i]['arqid'])) {
                
                if ($t === 3) {
                    $tabelaFotos .= '</tr><tr>';
                    $t=0;
                }
                
                $fotoDescribe = (!empty($fotos[$i]['arqdescricao'])) ? "<br>{$fotos[$i]['arqdescricao']}" : '';
                $tabelaFotos .= "<td align='center'>
                    <img 
                    src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotos[$i]['arqid']}' 
                    hspace='3' 
                    vspace='3' 
                    style='width:225px; height:225px;' 
                    />
                    $fotoDescribe
                </td>";
               
               $t++;
            }
        }
        $tabelaFotos .= '</table>';

    } else {

        $tabelaFotos = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'><tr>
            <td align='center' colspan='2' style='color:#ee0000'>Não existem fotos cadastradas para a obra.</td>
        </tr></table>";
    }			

    return "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>
        <tr>
            <td class='subtitulocentro' colspan='2' height='25px;'>Galeria de Fotos</td>
        </tr>
        <tr>
            <td colspan='2'>$tabelaFotos</td>
        </tr>
    </table>";
}

function getExecucaoOrcamentaria($obrID) {
    global $db;
    
    $strSQL = "SELECT * FROM obras2.execucaoorcamentaria WHERE obrid = %d AND teoid = %d AND eorstatus = '%s'";
    
    $stmt = sprintf($strSQL, (int)$obrID, 1, 'A'); 
    $dadosExecOrc = $db->carregar($stmt);
    
    echo '<pre>';
    print_r($stmt); exit;
    
    $tabelaOrcamento = '<table class="tabela" cellSpacing="1" cellPadding="3" align="center">';
	
    if ($dadosExecOrc) {
        
        $nEocvlrtotal = $dadosExecOrc['eocvlrcapital'] + $dadosExecOrc['eocvlrcusteio'];
        $eocvlrtotal  = simec_number_format($nEocvlrtotal, 2, ',', '.');
        
        $tabelaOrcamento .= "<tr>
            <td class='subtitulocentro'>Capital (R$)</td>
            <td class='subtitulocentro'>Custeio (R$)</td>
            <td class='subtitulocentro'>Total (R$)</td>
        </tr>
            <tr bgcolor='#ffffff'>
            <td align='center'>".simec_number_format($dadosExecOrc['eocvlrcapital'], 2, ',', '.')."</td>
            <td align='center'>".simec_number_format($dadosExecOrc['eocvlrcusteio'], 2, ',', '.')."</td>
            <td align='center'>{$eocvlrtotal}</td>
        </tr>";
    } else {
        $tabelaOrcamento .= '<tr>
            <td align="center" style="color:#ee0000">Não existem Orçamentos cadastrados para a obra.</td>
        </tr>';
    }
    $tabelaOrcamento .= '</table>';
    
    $tabelaDetalhamento = getDetalhamentoOrcamentario($dadosExecOrc/*, $nEocvlrtotal*/);
    
    return array(
        'detalhamento' => $tabelaDetalhamento
      , 'orcamento' => $tabelaOrcamento
    );
}

function getDetalhamentoOrcamentario($dadosExecOrc/*, $nEocvlrtotal*/) {
    
    global $db;
    
    if (array_key_exists('eorid', $dadosExecOrc) && !empty($dadosExecOrc['eorid'])) {
        $strSQL = "SELECT 
            ioctipodetalhamento,
            iocvalor, 
            iocdtposicao				
        FROM obras2.itensexecucaoorcamentaria
        WHERE eorid = %d
        ORDER BY iocdtposicao";
        $stmt = sprintf($strSQL, (int) $dadosExecOrc['eorid']);
        $dadosItensExec = $db->carregar($stmt);
    }
    
    $tabelaDetalhamento = '<table class="tabela" cellSpacing="1" cellPadding="3" align="center">';

    if ($dadosItensExec) {

        $countItensExec = count($dadosItensExec);
        $tabelaDetalhamento .= "<tr>
            <td class='subtitulocentro'>Data</td>
            <td class='subtitulocentro'>Valor Empenhado (R$)</td>
            <td class='subtitulocentro'>Valor Liquidado (R$)</td>
            <td class='subtitulocentro'>% Empenhado</td>
            <td class='subtitulocentro'>% Liquidado</td>
        </tr>";

        for ($i=0; $i<$countItensExec; $i++) {

            $cor = ($i%2) ? '#e0e0e0' : '#ffffff';

//            $perEmpenhado = ($nEocvlrtotal > 0) ? ( $dadosItensExec[$i]['eocvlrempenhado'] / $nEocvlrtotal ) * 100 : 0.00;
//            $perLiquidado = ($nEocvlrtotal > 0) ? ( $dadosItensExec[$i]['eocvlrliquidado'] / $nEocvlrtotal ) * 100 : 0.00;
//
//            $totEmpenhado = $totEmpenhado + $dadosItensExec[$i]['eocvlrempenhado'];
//            $totLiquidado = $totLiquidado + $dadosItensExec[$i]['eocvlrliquidado'];
//
//            $totPerEmpenhado = $totPerEmpenhado + $perEmpenhado;
//            $totPerLiquidado = $totPerLiquidado + $perLiquidado;

            $tabelaDetalhamento .= "<tr bgcolor='{$cor}'>
                <td align='center'>".formata_data($dadosItensExec[$i]['eocdtposicao'])."</td>
                <td align='right'>"./*simec_number_format($dadosItensExec[$i]['eocvlrempenhado'], 2, ',', '.').*/"</td>
                <td align='right'>"./*simec_number_format($dadosItensExec[$i]['eocvlrliquidado'], 2, ',', '.').*/"</td>
                <td align='right'>"./*simec_number_format($perEmpenhado, 2, ',', '.').*/"</td>
                <td align='right'>"./*simec_number_format($perLiquidado, 2, ',', '.').*/"</td>
            </tr>";

        }

        $tabelaDetalhamento .= "<tr bgcolor='#c0c0c0'>
            <td align='right'><b>Total</b></td>
            <td align='right'>"./*simec_number_format($totEmpenhado, 2, ',', '.').*/"</td>
            <td align='right'>"./*simec_number_format($totLiquidado, 2, ',', '.').*/"</td>
            <td align='right'>"./*simec_number_format($totPerEmpenhado, 2, ',', '.').*/"</td>
            <td align='right'>"./*simec_number_format($totPerLiquidado, 2, ',', '.').*/"</td>
        </tr>";
    } else {
        $tabelaDetalhamento .= '<tr>
            <td align="center" style="color:#ee0000">Não existem Detalhamentos Orçamentários cadastrados para a obra.</td>
        </tr>';
    }
    $tabelaDetalhamento .= '</table>';
    
    return $tabelaDetalhamento;
}

function getComplementoFotos($obrID) {
    
    $return = getExecucaoOrcamentaria($obrID);
    $tabela = '<table class="tabela" cellSpacing="1" cellPadding="3" align="center">';
    
    if (array_key_exists('orcamento', $return)) {
        $tabela .= "<tr>
            <td class='subtitulocentro' colspan='2' height='25px;'>Orçamento para a Obra</td>
        </tr>
        <tr>
            <td colspan='2'>{$return['orcamento']}</td>
        </tr>";
    }
    if (array_key_exists('detalhamento', $return)) {   
        $tabela .= "<tr>
            <td class='subtitulocentro' colspan='2' height='25px;'>Detalhamento Orçamentário</td>
        </tr>
        <tr>
            <td colspan='2'>{$return['detalhamento']}</td>
        </tr>";
    }
    $tabela .= '</table>';
    return $tabela;
}

/*
 * RESTRIÇÕES E PROVIDENCIAS
 */
function getRestricoesProvidencias($obrID) {
    global $db;
    $strSQL = "SELECT
        CASE WHEN fsrid is not null THEN fsrdsc ELSE 'Não Informada' END as fase,
        rstdsc,
        tprdsc,
        rstdscprovidencia,
        to_char(rstdtprevisaoregularizacao,'DD/MM/YYYY') as rstdtprevisaoregularizacao,
        CASE WHEN rstsituacao = true THEN to_char(rstdtsuperacao,'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao
    FROM
        obras2.restricao 
    INNER JOIN 
        obras.tiporestricao USING (tprid)
    LEFT JOIN
        obras.faserestricao USING (fsrid) 
    WHERE
        rststatus = 'A' AND
        obrid = %d";
    $stmt = sprintf($strSQL, (int) $obrID);
    $dadosRestricoes = $db->carregar($stmt);
	
    $tabelaRestricoes = '<table class="tabela" cellSpacing="1" cellPadding="3" align="center">';

    if ($dadosRestricoes) {
        $tabelaRestricoes .= "<tr>
            <td class='subtitulocentro'>Fase da Restrição</td>
            <td class='subtitulocentro'>Tipo de Restrição</td>
            <td class='subtitulocentro'>Descrição</td>
            <td class='subtitulocentro'>Providência</td>
            <td class='subtitulocentro'>Previsão da Providência</td>
            <td class='subtitulocentro'>Superação</td>
        </tr>";

        $countRestricoes = count($dadosRestricoes);

        for ($i=0; $i<$countRestricoes; $i++) {

            $cor = $i % 2 ? '#e0e0e0' : '#ffffff';

            $tabelaRestricoes .= "<tr bgcolor='{$cor}'>
                    <td align='center'>{$dadosRestricoes[$i]['fase']}</td>
                    <td align='center'>{$dadosRestricoes[$i]['tprdsc']}</td>
                    <td>{$dadosRestricoes[$i]['rstdsc']}</td>
                    <td>{$dadosRestricoes[$i]['rstdscprovidencia']}</td>
                    <td align='center'>{$dadosRestricoes[$i]['rstdtprevisaoregularizacao']}</td>
                    <td align='center'>{$dadosRestricoes[$i]['rstdtsuperacao']}</td>
              </tr>";
        }

    } else {
        $tabelaRestricoes .= "<tr>
            <td align='center' style='color:#ee0000'>Não existem Restrições e Providências cadastradas para a obra.</td>
        </tr>";
    }

    $tabelaRestricoes .= '</table>';

    return "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>
     <tr>
        <td class='subtitulocentro' colspan='2' height='25px;'>Restrições e Providências</td>
    </tr>
    <tr>
        <td colspan='2'>{$tabelaRestricoes}</td>
    </tr>
    </table>";
}

function queryVistoria($obrID, $supID) {
    global $db;
    
    $supID = trim($supID);
    $supID = substr($supID, 1, strlen($supID)-2);
    $where = (!empty($supID)) ? "s.supid IN ({$supID}) AND " : '';
    
    $strSQL = "SELECT DISTINCT
        s.supid AS vistoria,
        s.suprealizacao AS responsavel,
        u.usunome AS inseridopor,
        to_char(s.supdata,'DD/MM/YYYY') AS dtvistoria,
        CASE WHEN s.supvistoriador IS NOT NULL THEN e.entnome ELSE 'Não informado' END AS vistoriador,
        sa.stadesc AS situacao,
        CASE WHEN s.supprojespecificacoes = 't' THEN 'Sim' ELSE 'Não' END AS projetoespecificacoes,
        CASE WHEN s.supplacaobra = 't' THEN 'Sim' ELSE 'Não' END AS placaobra,
        CASE WHEN s.supdiarioobra = 't' THEN 'Sim' ELSE 'Não' END AS diarioobra,
        CASE WHEN s.supplacalocalterreno = 't' THEN 'Sim' ELSE 'Não' END AS placalocalterreno,
        CASE WHEN s.supvalidadealvara = 't' THEN 'Sim' ELSE 'Não' END AS validadealvara,
        oq.qlbdesc AS qualidadeobra,
        od.dcndesc AS desempenho,
        CASE WHEN s.supobs != '' THEN s.supobs ELSE 'Não informado' END AS observacao
    FROM
        obras2.supervisao s
    INNER JOIN obras2.situacaoatividade sa ON (sa.staid = s.staid) 
    INNER JOIN seguranca.usuario u ON (u.usucpf = s.usucpf)
    LEFT JOIN entidade.entidade e ON (e.entid = s.supvistoriador)
    LEFT JOIN obras2.realizacaosupervisao rs ON (rs.rsuid = s.rsuid)
    LEFT JOIN obras2.cronograma cro ON (cro.obrid = %d) AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
    LEFT JOIN obras2.itenscomposicaoobra i ON (i.croid = cro.croid AND i.obrid = cro.obrid)
    LEFT JOIN obras2.itenscomposicao ic ON (ic.itcid = i.itcid)
    LEFT JOIN obras2.qualidadeobra oq ON (oq.qlbid = s.qlbid)
    LEFT JOIN obras2.desempenhoconstrutora od ON (od.dcnid = s.dcnid)
    WHERE
        %s
        s.obrid = %d AND
        s.supstatus = 'A'
        and (i.icovigente='A' OR i.icovigente IS NULL) 
    ORDER BY 
        s.supid ASC";
    $stmt = sprintf($strSQL, (int)$obrID , (string)$where, (int)$obrID);
    return $db->carregar($stmt);
}

function getVistorias($obrID, $supID) {
    
    //$totVistorias = queryVistoria($obrID, $supID);
    $vistoria = new Supervisao();
    $totVistorias = $vistoria->getSupervisao($obrID, $supID);

    if ($totVistorias) {
	
        $vistoriasCount = count($totVistorias);

        for ($i = 0; $i<$vistoriasCount; $i++) {
            
            //Itens vistoria
            $tabelaItensVistoria = montaTabelaItensVistoria($totVistorias[$i]['supid']);

            //Fotos vistorias
            $fotosVistoria = fotosVistoria($totVistorias[$i]['supid'], $_POST);
            $tabelaFotosVistoria = montaTabelaFotosVistoria($fotosVistoria);
            
            $tabelaVistorias .= "<tr>"
            .  "    <td align='center' colspan='2'>"
            .  "        <table class='tabela' cellSpacing='1' cellPadding='3' align='center'>"
            .  "            <tr>"
            .  "                <td class='subtitulocentro' colspan='2'>Vistoria nº ". ($i + 1) . "</td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita' width='290px'>Responsável</td>"
            .  "                <td bgcolor='#f5f5f5'>"
            .  "                    {$totVistorias[$i]['responsavel']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita' width='190px'>Inserido Por</td>"
            .  "                <td>"
            .  "                    {$totVistorias[$i]['usunome']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita' width='190px'>Data da Vistoria</td>"
            .  "                <td bgcolor='#f5f5f5'>"
            .  "                    {$totVistorias[$i]['dtvistoria']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita'>Nome do Vistoriador</td>"
            .  "                <td>"
            .  "                    {$totVistorias[$i]['vistoriador']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita'>Situação atual</td>"
            .  "                <td bgcolor='#f5f5f5'>"
            .  "                     {$totVistorias[$i]['stadesc']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita'>Projeto/Especificações</td>"
            .  "                <td>"
            .  "                    {$totVistorias[$i]['projetoespecificacoes']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita'>Placa da Obra</td>"
            .  "                <td bgcolor='#f5f5f5'>"
            .  "                    {$totVistorias[$i]['placaobra']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita'>Diário da Obra Atualizado</td>"
            .  "                <td>"
            .  "                    {$totVistorias[$i]['diarioobra']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita'>Placa Indicativa do Programa/Dados da obra</td>"
            .  "                <td bgcolor='#f5f5f5'>"
            .  "                    {$totVistorias[$i]['placalocalterreno']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita'>Validade do Alvará da Obra</td>"
            .  "                <td>"
            .  "                    {$totVistorias[$i]['validadealvara']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita'>Qualidade de Execução da Obra/Projeto</td>"
            .  "                <td bgcolor='#f5f5f5'>"
            .  "                    {$totVistorias[$i]['qualidadeobra']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita'>Desempenho da Construtora/Projetista</td>"
            .  "                <td>"
            .  "                    {$totVistorias[$i]['desempenho']}"
            .  "                </td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td colspan='2'><b>Detalhamento de Vistoria e Acompanhamento</b></td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td colspan='2'><pre>{$tabelaItensVistoria}</td>"
            .  "            </tr>"
            .  "            <tr>"
            .  "                <td class='subtitulodireita'>Relatório Técnico do Acompanhamento</td>"
            .  "                <td bgcolor='#f5f5f5' align='justify'>"
            .  "                    {$totVistorias[$i]['supobs']}"
            .  "                </td>"
            .  "            </tr>"
            .  "	    <tr>"
            .  "                <td colspan='2'><pre>{$tabelaFotosVistoria}</pre></td>"
            .  "            </tr>"
            .  "	</table>"
            .  "    </td>"
            .  "</tr>";
            
        }
    } else {
        $tabelaVistorias .= '<table align="center"><tr>
            <td colspan="2" align="center" style="color:#ee0000">Não existem vistorias cadastradas para a obra.</td>
        </tr></table>';
    }
    
    return "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>
        <tr>
            <td class='subtitulocentro' colspan='2' height='25px;'>Supervisão e Vistoria</td>
        </tr>
        <tr>
            <td colspan='2'>{$tabelaVistorias}</td>
        </tr>
    </table>";
}

function findVistoria($vistoria) {
    
    global $db;
    
    $strSQL = "SELECT DISTINCT 
        i.icoid,
        ic.itcdesc,
        i.icovlritem,
        i.icopercsobreestrutura,
        TO_CHAR(i.icodtinicioitem, 'DD/MM/YYYY') AS inicio,
        TO_CHAR(i.icodterminoitem, 'DD/MM/YYYY') AS termino,
        i.icopercexecutado,
        si.spivlrinfsupervisor AS spivlrinfsupervisor,
        si.spivlritemexecanterior,
        si.spivlritemsobreobraexecanterior,
        si.supid,
        i.icopercprojperiodo,
        i.icoordem,
        si.spivlrfinanceiroinfsupervisor
    FROM
        obras2.supervisao s
    INNER JOIN 
        obras2.situacaoatividade sa ON (sa.staid = s.staid) 
    INNER JOIN 
        seguranca.usuario u ON (u.usucpf = s.usucpf)
    LEFT JOIN 
        entidade.entidade e ON (e.entid = s.supvistoriador)
    LEFT JOIN 
        obras2.realizacaosupervisao rs ON (rs.rsuid = s.rsuid)
    LEFT JOIN
        obras2.cronograma cro ON cro.obrid = s.obrid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
    LEFT JOIN 
        obras2.itenscomposicaoobra i ON (i.croid = cro.croid AND i.obrid = cro.obrid)
    LEFT JOIN 
        obras2.itenscomposicao ic ON (i.itcid = ic.itcid)
    LEFT JOIN 
        obras2.qualidadeobra oq ON (oq.qlbid = s.qlbid)
    LEFT JOIN 
        obras2.desempenhoconstrutora od ON (od.dcnid = s.dcnid)
    LEFT JOIN 
        obras2.supervisaoitem si ON (si.supid = s.supid AND si.icoid = i.icoid)
    WHERE
        s.supid = %d AND
        s.supstatus = 'A'
        AND si.spivlrinfsupervisor IS NOT NULL
        AND si.spivlritemexecanterior IS NOT NULL
        AND si.spivlritemsobreobraexecanterior IS NOT NULL
    ORDER BY 
            i.icoordem";
    $stmt = sprintf($strSQL, (int)$vistoria);
    return $db->carregar($stmt);
}

function fotosVistoria($supid, $post) {
    global $db;
    
    $fotosSelecionadas = $post['fotoselecionadas'];
    $whereFotos = (!empty($fotosSelecionadas)) ? "fo.arqid IN ({$fotosSelecionadas}) AND " : '';
    
    $strSQL = "SELECT 
        arqnome, 
        arq.arqid, 
        arq.arqextensao, 
        arq.arqtipo, 
        arq.arqdescricao 
    FROM 
        public.arquivo arq
    INNER JOIN 
        obras2.fotos fo ON (arq.arqid = fo.arqid)
    WHERE
        %s
        supid = %d
    ORDER BY 
        fotordem";
    $stmt = sprintf($strSQL, (string) $whereFotos, (int) $supid);
    //return $stmt;
    return $db->carregar($stmt);
}

function montaTabelaFotosVistoria($fotosVistoria) {
    
    if (count($fotosVistoria)) {

        $tabelaFotosVistoria = '<table class="tabela" cellSpacing="1" cellPadding="3" align="center"><tr>';
        $countFotosVistoria = count($fotosVistoria);
        $z = 0;
        
        for ($a=0; $a<$countFotosVistoria; $a++) {

            if (!empty($fotosVistoria[$a]['arqid'])) {
                
                $tabelaFotosVistoria .= "<td align='center'>
                <img 
                    src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotosVistoria[$a]['arqid']}'
                    hspace='3' 
                    vspace='3' 
                    style='width:125px; 
                    height:125px;'
                />
                <br>{$fotosVistoria[$a]['arqdescricao']}
                </td>";
                
                $z++;
                
                if ($z === 4) {
                    $tabelaFotosVistoria .= '</tr><tr>';
                    $z = 0;
                }
            }
        }
        
        $tabelaFotosVistoria .= '</tr>';
        
    } else { 
        $tabelaFotosVistoria = '<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td align="center" style="color:#ee0000" colspan="2">Não existem fotos cadastradas para esta vistoria.</td>
        </tr>';
    }

    $tabelaFotosVistoria .= '</table>';
    
    return $tabelaFotosVistoria;
}

function montaTabelaItensVistoria($vistoria_item) {

    $tabelaItensVistoria = '<table class="tabela" cellSpacing="1" cellPadding="3" align="center">';
    $dadosItensVistoria = findVistoria($vistoria_item);

    if ($dadosItensVistoria) {

        $totalValor = 0;
        $totalPercObra = 0;
        $totalPercObraAnt = 0;
        $totalPercObraAtual = 0;
        $countItensVistoria = count($dadosItensVistoria);

        $tabelaItensVistoria .= "<tr>
            <td colspan='1' rowspan='2' class='subtitulocentro'>Item da Obra</td>
            <td colspan='1' rowspan='2' class='subtitulocentro'>Valor (R$)</td>
            <td colspan='1' rowspan='2' class='subtitulocentro'>(%) Sobre a Obra</td>
            <td colspan='1' rowspan='2' class='subtitulocentro'>Data de Início</td>
            <td colspan='1' rowspan='2' class='subtitulocentro'>Data de Término</td>
            <td colspan='2' rowspan='1' class='subtitulocentro'>Última Vistoria</td>
            <td colspan='2' rowspan='1' class='subtitulocentro'>Vistoria Atual</td>
        </tr>
        <tr>
            <td class='subtitulocentro'>(%) do Item já Executado</td>
            <td class='subtitulocentro'>(%) do Item já Executado <br/> sobre a Obra</td>
            <td class='subtitulocentro'>(%) Supervisão</td>
            <td class='subtitulocentro'>(%) do Item já Executado sobre a  <br/> Obra após Supervisão</td>
        </tr>";
        $totalValor = 0;
        for ($k=0; $k<$countItensVistoria; $k++) {
            $totalValor = $totalValor + $dadosItensVistoria[$k]['icovlritem'];
        }
        for ($k=0; $k<$countItensVistoria; $k++) {

            $cor = ($k%2) ? '#e0e0e0' : '#ffffff'; 
            if($totalValor != 0){
                $supervisao_exec_sobre_obra = (((float)$dadosItensVistoria[$k]['spivlrfinanceiroinfsupervisor'] * 100)/$totalValor);
            }else{
                $supervisao_exec_sobre_obra = 0;
            }
            
//            $totalValor = $totalValor + $dadosItensVistoria[$k]['icovlritem'];
            $totalPercObra = $totalPercObra + $dadosItensVistoria[$k]['icopercsobreobra'];
            $totalPercObraAnt = $totalPercObraAnt + $dadosItensVistoria[$k]['spivlritemsobreobraexecanterior'];
            $totalPercObraAtual = $totalPercObraAtual + $supervisao_exec_sobre_obra;
//$dadosItensVistoria[$k]
            $tabelaItensVistoria .= "<tr bgcolor='{$cor}'>
                <td>{$dadosItensVistoria[$k]['itcdesc']}</td>
                <td align='right'>".simec_number_format($dadosItensVistoria[$k]['icovlritem'], 2, ',', '.')."</td>
                <td align='right'>".simec_number_format($dadosItensVistoria[$k]['icopercsobreobra'], 2, ',', '.')."</td>
                <td align='center'>{$dadosItensVistoria[$k]['inicio']}</td>
                <td align='center'>{$dadosItensVistoria[$k]['termino']}</td>
                <td align='right'>".simec_number_format($dadosItensVistoria[$k]['spivlritemexecanterior'], 2, ',', '.')."</td>
                <td align='right'>".simec_number_format($dadosItensVistoria[$k]['spivlritemsobreobraexecanterior'], 2, ',', '.')."</td>
                <td align='right'>".simec_number_format($dadosItensVistoria[$k]['spivlrinfsupervisor'], 2, ',', '.')."</td>
                <td align='right'>".simec_number_format($supervisao_exec_sobre_obra, 2, ',', '.')."</td>
            </tr>";

        }

        $totalPercObra = ($totalPercObra > 100.00) ? 100.00 : $totalPercObra;
        $tabelaItensVistoria .= "<tr bgcolor='#d0d0d0'>
            <td align='right'><b>Total</b></td>
            <td align='right'><b>".simec_number_format($totalValor, 2, ',', '.')."</b></td>
            <td align='right'><b>".simec_number_format($totalPercObra, 2, ',', '.')."</b></td>
            <td align='right'><b></b></td>
            <td align='right'><b></b></td>
            <td align='right'><b></b></td>
            <td align='right'><b>".simec_number_format($totalPercObraAnt, 2, ',', '.')."</b></td>
            <td align='right'><b></b></td>
            <td align='right'><b>".simec_number_format($totalPercObraAtual, 2, ',', '.')."</b></td>
        </tr>";

    } else {
        $tabelaItensVistoria .= '<tr>
            <td align="center" style="color:#ee0000">Não existem itens cadastradas para esta vistoria.</td>
        </tr>';
    }
    
    $tabelaItensVistoria .= '</table>';

    return $tabelaItensVistoria;
}
