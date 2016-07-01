<?php 

function pegaQrpidMds( $entcodent )
{
	global $db;
    
    $sql = "SELECT
            	que.qrpid
            FROM
            	proinfantil.suplementacaomds que
            INNER JOIN 
            	questionario.questionarioresposta qr ON qr.qrpid = que.qrpid
            WHERE
            	que.entcodent = '{$entcodent}' 
            	AND qr.queid = ".QUESTIONARIOMDS;

    $qrpid = $db->pegaUm( $sql );
    
    if(!$qrpid)
    {
    	
        $sql = "SELECT
        			ent.entnome
        		FROM
        			proinfantil.suplementacaomds mds
        		INNER JOIN
        			entidade.entidade ent ON ent.entcodent = mds.entcodent
				WHERE
					mds.entcodent = '".$entcodent."'";

        $titulo = $db->pegaUm( $sql );
        
        $arParam = array ( "queid" => QUESTIONARIOMDS, "titulo" => "Proinfância - Suplementação MDS (".$titulo.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        
        $sql = "INSERT INTO proinfantil.suplementacaomds (qrpid, entcodent) VALUES ({$qrpid},{$entcodent})";
        $db->executar( $sql );
        $db->commit();
    }
    
    return $qrpid;
}

function cabecalhoMds()
{		
	global $db;
	
	if($_SESSION['proinfantil']['mds']['entcodent']){
		
		$sql = "select 
					ent.entnome,
					mun.estuf,
					mun.mundescricao 
				from 
					entidade.entidade ent
				inner join 
					entidade.endereco ede on ede.entid = ent.entid
				left join 
					territorios.municipio mun on mun.muncod = ede.muncod 
				where 
					ent.entcodent = '{$_SESSION['proinfantil']['mds']['entcodent']}'";
		
		$rs = $db->pegaLinha($sql);
		
		if($rs){
			echo '<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
					<tr>
						<td width="280" class="subtituloDireita">Escola</td>
						<td>'.$rs['entnome'].'</td>
					</tr>
					<tr>
						<td class="subtituloDireita">UF</td>
						<td>'.$rs['estuf'].'</td>
					</tr>
					<tr>
						<td class="subtituloDireita">Município</td>
						<td>'.$rs['mundescricao'].'</td>
					</tr>
				  </table>
			';
		}
	}
}

function cabecalhoMunicipio()
{
	global $db;
	
	$sql = "select 
				estuf,
				mundescricao
			from
				territorios.municipio
			where
				muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'";
	
	$rs = $db->pegaLinha($sql);
	
	$html = '';
	if($rs){
		$html .= '<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
					<tr>
						<td width="50%" class="subtituloDireita">UF</td>
						<td width="50%">'.$rs['estuf'].'</td>
					</tr>
					<tr>
						<td class="subtituloDireita">Município</td>
						<td>'.$rs['mundescricao'].'</td>
					</tr>
				 </table>';
	}
	echo $html;
}

function salvarFotosSalaMds()
{
	global $db;
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	extract($_POST);	
	
	if( $_FILES['arquivo']['tmp_name'] ){
		$arrCampos = array(
						"salid" => $salid,
						"mdsid" => $mdsid,
						"usucpf" => "'{$_SESSION['usucpf']}'",
						"fotstatus" => "'A'",
						"fotdatainclusao" => "now()"
					      );					      			     
		$file = new FilesSimec("fotos", $arrCampos, "proinfantil");
		$file->setUpload($arqdescricao, "arquivo");
		$db->sucesso('suplementacaomds/formFotosSuplementacao');
		die;
	}else{
		$_SESSION['proinfantil']['mgs'] = "Não foi possível realizar a operação!";
		$db->sucesso('suplementacaomds/formFotosSuplementacao');
		die;
	}
	
}

function removerFotoSalaMds()
{
	global $db;
	$arqid = $_GET['arqid'];
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$file = new FilesSimec("fotos",array(),"proinfantil");
	$file->excluiArquivoFisico($arqid);
	$sql = "delete from proinfantil.fotos where arqid = $arqid;";
	$sql.= "delete from public.arquivo where arqid = $arqid;";
	$db->executar($sql);
	$db->commit();
}

function recuperarMdsidPorEntcodent()
{
	global $db;
	
	$sql = "select 
				mdsid 
			from 
				proinfantil.suplementacaomds 
			where 
				entcodent = '{$_SESSION['proinfantil']['mds']['entcodent']}'";
	
	return $db->pegaUm($sql);
}

// INICIO FUNÇÕES DO WORKFLOW

function criaDocumento( $cpmid ) {
	
	global $db;
	
	if(empty($cpmid)) return false;
	
	$docid = pegaDocid( $cpmid );
	
	if( !$docid ){
				
		$tpdid = WF_TPDID_SUPLEMENTACAO_MDS;
		
		$docdsc = "Cadastramento suplementação MDS";
		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );		
		
		if($cpmid) {
			$sql = "UPDATE proinfantil.mdsdadoscriancapormunicipio SET 
					 docid = ".$docid." 
					WHERE
					 cpmid = ".$cpmid."
					 and cpmano = ".($_SESSION['exercicio'] - 1);

			$db->executar( $sql );		
			$db->commit();
			return $docid;
		}else{
			return false;
		}
	}
	else {
		return $docid;
	}
}

function pegaDocid( $cpmid ) {
	
	global $db;
	
	$cpmid = (integer) $cpmid;	
	
	$sql = "SELECT docid FROM proinfantil.mdsdadoscriancapormunicipio WHERE cpmid  = " . $cpmid . " and cpmano = ".($_SESSION['exercicio'] - 1);
	
	return (integer) $db->pegaUm( $sql );
}

function pegaEstadoAtual( $docid ) {
	
	global $db; 
	
	if($docid) {
		$docid = (integer) $docid;
		 
		$sql = "
			select
				ed.esdid
			from 
				workflow.documento d
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
		$estado = $db->pegaUm( $sql );
		 
		return $estado;
	} else {
		return false;
	}
}

function condicaoEnviarParaAnaliseMDS()
{
	global $db;
	
	$sql = "select distinct
	            ent.entcodent,
	            ent.entnome,
	            coalesce(pro.prcqtdalunoinfantilintegral,0) as prcqtdalunoinfantilintegral,
	            coalesce(pro.prcqtdalunoinfantilparcial,0) as prcqtdalunoinfantilparcial,
	            coalesce(mdsquantidadepbfparcial,0)+coalesce(mdsquantidadepbfintegral,0) as mdsquantidadepbf,
	            coalesce(mdsquantidadepbfparcial,0) as mdsquantidadepbfparcial,
	            coalesce(mdsquantidadepbfintegral,0) as mdsquantidadepbfintegral
	        from
	            entidade.entidade ent
	        inner join
	            entidade.endereco ede on ede.entid = ent.entid
	        inner join
	            territorios.municipio mun on mun.muncod = ede.muncod
	        inner join 
	        	proinfantil.procenso pro on pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
	        left join 
	        	proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'         
			where 
				ent.entstatus = 'A'
			and 
				ent.entcodent is not null
			and 
				ent.tpcid in (1,2,3)			
			and
				mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
			order by 
				ent.entnome";
	
	$rsCreches = $db->carregar($sql);
	
	$sql = "select				            
	            sum(pro.prcqtdalunoinfantilintegral+pro.prcqtdalunoinfantilparcial) as alunos_mat,
	            sum(coalesce(sup.mdsquantidadepbfparcial,0)+coalesce(sup.mdsquantidadepbfintegral,0)) as alunos_pbf,
	            coalesce(cpmnumcriancascompbf,0) as alunos_mun,
	            cpmqtdcrianca03mun as alunos_mes
	        from
	            entidade.entidade ent
	        inner join
	            entidade.endereco ede on ede.entid = ent.entid
	        inner join
	            territorios.municipio mun on mun.muncod = ede.muncod					        
	        inner join 
	        	proinfantil.procenso pro on pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
	        left join 
	        	proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'
	        left join 
	        	proinfantil.mdsdadoscriancapormunicipio dcm on dcm.muncod = mun.muncod  and dcm.cpmano = ({$_SESSION['exercicio']} - 1)        
			where 
				ent.entstatus = 'A'
			and 
				ent.entcodent is not null									
			and
				mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
			group by 
				mun.muncod,cpmnumcriancascompbf,cpmqtdcrianca03mun";
	
		$rs = $db->pegaLinha($sql);		
		
		if($rsCreches){
			foreach($rsCreches as $creches){
				if($creches['mdsquantidadepbfintegral'] > $creches['prcqtdalunoinfantilintegral']){
					return false;
				}				
				if($creches['mdsquantidadepbfparcial'] > $creches['prcqtdalunoinfantilparcial']){
					return false;
				}
			}
		}
		
		if(!$rs['alunos_pbf']){
			return false;
		}
		
		if($rs['alunos_pbf'] > $rs['alunos_mun']){
			return false;
		}
		
		if($rs['alunos_pbf'] > $rs['alunos_mat']){
			return false;
		}
		
		return true;
		
}

// Funções Aba Pagamento por lote

function listaMunicipiosSemLoteMDS( $post ){
	
	global $db;
	
	extract($post);
	
	$arWhere = Array('esd.esdid = 543','dcm.lotid IS NULL');
	
	if($estuf){
		$arWhere[] = "mun.estuf = '{$estuf}'";
	}
	
	if($mundescricao){
		$arWhere[] = "mun.mundescricao ilike '%{$mundescricao}%'";
	}
	
	$sql = "SELECT 
				'<center>
					<input type=\"checkbox\" name=\"cpmid[]\" value=\"'|| dcm.cpmid ||'\" /> 
					<input type=\"hidden\" name=\"docid['|| dcm.cpmid ||']\" value=\"'|| dcm.docid ||'\" />
				</center>' as cpmid,
				mun.muncod as codigo,
				mun.mundescricao||'/'||mun.estuf as municipio,
				esd.esddsc
			FROM 
				territorios.municipio mun
			INNER JOIN proinfantil.mdsdadoscriancapormunicipio 	dcm ON dcm.muncod = mun.muncod  and dcm.cpmano = ({$_SESSION['exercicio']} - 1)
			INNER JOIN workflow.documento doc ON doc.docid = dcm.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
				 ".implode(' AND ', $arWhere)."
			ORDER BY
				mun.estuf, mun.mundescricao";
	
	$cabecalho = array('Ação &nbsp;<input type=\'checkbox\' class=\'todos\'>', 'Código IBGE', 'Município', 'Estado');
	$db->monta_lista_simples($sql, $cabecalho, 6000, 1, '', '', '', '');
}

function listaMunicipiosComLoteMDS( $post ){
	
	global $db;
	
	extract($post);
	
	$perfil = pegaPerfil($_SESSION['usucpf']);
	$acaoSQLDisabled = " '<input type=\"checkbox\" disabled checked name=\"docid[]\" value=\"'|| doc.docid ||'\" />' ";
	
	if( $perfil == PERFIL_ANALISTA_PAGAMENTO || $db->testa_superuser() ){
		$acaoSQL = " '<input type=\"checkbox\" name=\"docid[]\" value=\"'|| doc.docid ||'\" />' ";
	} else {
		$acaoSQL = " '<input type=\"checkbox\" disabled checked name=\"docid[]\" value=\"'|| doc.docid ||'\" />' ";
	}
	
	$arWhere = Array('1=1');
	if($estuf){
		$arWhere[] = "mun.estuf = '{$estuf}'";
	}
	
	if($mundescricao){
		$arWhere[] = "mun.mundescricao ilike '%{$mundescricao}%'";
	}
	
	if($lotid){
		$arWhere[] = "lot.lotid = $lotid";
	}
	
	$sql = "SELECT DISTINCT
				'<center>'||CASE WHEN esd.esdid = ".ESDID_MDS_AGUARDANDO_PAGAMENTO." THEN $acaoSQL ELSE $acaoSQLDisabled END||'</center>' as acao,
				iue.iuecnpj,
				mun.muncod as codigo,
				mun.mundescricao||'/'||mun.estuf as municipio,
				esd.esddsc,
				lot.lotid
			FROM 
				territorios.municipio mun
			INNER JOIN proinfantil.mdsdadoscriancapormunicipio 	dcm ON dcm.muncod = mun.muncod and dcm.cpmano = ({$_SESSION['exercicio']} - 1)
			INNER JOIN workflow.documento						doc ON doc.docid = dcm.docid
			INNER JOIN workflow.estadodocumento					esd ON esd.esdid = doc.esdid 					   
			INNER JOIN proinfantil.lote			   			   	lot ON lot.lotid = dcm.lotid
			left join par.instrumentounidade iu
				inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
			on iu.muncod = mun.muncod
			WHERE
				".implode(' AND ', $arWhere)."
			ORDER BY 
				3,2";
	
	$cabecalho = array('Selecione', 'CNPJ', 'Código IBGE','Município','Estado',"Lote");
	echo $db->monta_lista($sql, $cabecalho, 100000, 50, '', '', '', '');
	$sql = "SELECT DISTINCT
				doc.esdid
			FROM 
				proinfantil.lote lot
			INNER JOIN proinfantil.mdsdadoscriancapormunicipio 	dcm ON dcm.lotid = lot.lotid and dcm.cpmano = ({$_SESSION['exercicio']} - 1)
			LEFT  JOIN workflow.documento				doc ON doc.docid = dcm.docid AND doc.esdid = ".ESDID_MDS_AGUARDANDO_PAGAMENTO."
			LEFT  JOIN workflow.estadodocumento			esd ON esd.esdid = doc.esdid
			WHERE 
				lotstatus = 'A' AND lot.lotid = $lotid
			ORDER BY
				1";
	$testaLote = $db->pegaUm($sql);
	if( ($perfil == PERFIL_ANALISTA_PAGAMENTO || $db->testa_superuser()) && $testaLote != '' ){
?>	
	<center>
		<input type="button" value="Confirmar Pagamento por Município" class="tramitaDocid" style="cursor:pointer; width:150px; white-space: normal;"/>
	</center>
<?php 
	}
}

/*function geraLote( $request ){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	extract($request);
	
	if( $cpmid[0] != '' ){
		
		$sql = "INSERT INTO proinfantil.lote(usucpf) VALUES ('".$_SESSION['usucpf']."') RETURNING lotid;";
		$lotid = $db->pegaUm($sql);
		
		$sql = "UPDATE proinfantil.lote SET lotdsc = 'Lote: '||lotid WHERE lotid = $lotid;";
		foreach( $cpmid as $id ){
			$sql .= " UPDATE proinfantil.mdsdadoscriancapormunicipio SET lotid = $lotid WHERE cpmid = $id and cpmano = ({$_SESSION['exercicio']} - 1);";
			wf_alterarEstado( $docid[$id], AEDID_MDS_ENCAMINHAR_ANALISADO, 'Tramitação em Lote', array( ) );
		}
		$db->executar($sql);
		$db->commit();
	}
	echo "<script>
			alert('Lote criado.');
			window.location = 'proinfantil.php?modulo=suplementacaomds/pagamentoLote&acao=A';
		  </script>";
}*/

function geraLoteMDS( $request ){	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	extract($request);
	
	if( $cpmid[0] != '' ){
		$dataPortaria = explode('/', $lotdataportaria);
		$dia = $dataPortaria[0];
		$mes = $dataPortaria[1];
		$ano = $dataPortaria[2];
		$mes = mes_extenso($mes);
	
		$texto = '<p style="text-align: justify;"><strong>PORTARIA N&ordm;&nbsp;&nbsp;&nbsp;&nbsp;'.$lotnumportaria.'&nbsp;&nbsp;&nbsp;&nbsp; , &nbsp;&nbsp;&nbsp;&nbsp;DE&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dia.'&nbsp;&nbsp;&nbsp;&nbsp; DE &nbsp;&nbsp;'.$mes.'&nbsp;&nbsp;DE '.$ano.'.</strong></p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify; padding-left: 360px;">Autoriza o Fundo Nacional de Desenvolvimento da Educa&ccedil;&atilde;o - FNDE a realizar a transfer&ecirc;ncia de recurso financeiro suplementar aos Munic&iacute;pios e o Distrito Federal que pleitearam e est&atilde;o aptos para pagamento, conforme Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 17, de 16 de maio de 2013.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;"><strong>O SECRET&Aacute;RIO DE EDUCA&Ccedil;&Atilde;O B&Aacute;SICA</strong>, no uso das atribui&ccedil;&otilde;es, resolve:</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 1&ordm; Divulgar os munic&iacute;pios e o Distrito Federal que est&atilde;o aptos a receber o pagamento do recurso financeiro suplementar &agrave; manuten&ccedil;&atilde;o e ao desenvolvimento da educa&ccedil;&atilde;o infantil para atender crian&ccedil;as de zero a 48 meses, matriculadas em creches p&uacute;blicas ou conveniadas com o poder p&uacute;blico, informadas no Censo Escolar da Educa&ccedil;&atilde;o B&aacute;sica do ano anterior e cujas fam&iacute;lias sejam benefici&aacute;rias do Programa Bolsa Fam&iacute;lia, de que trata a Lei n&ordm; 12.722 de 3 de outubro de 2012, e conforme informa&ccedil;&otilde;es declaradas pelos munic&iacute;pios e Distrito Federal no SIMEC &ndash; M&oacute;dulo E.I. Manuten&ccedil;&atilde;o &ndash; Suplementa&ccedil;&atilde;o de Creches MDS.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 2&ordm; Autorizar o FNDE/MEC a realizar a transfer&ecirc;ncia de recursos financeiros suplementar aos munic&iacute;pios e Distrito Federal, conforme destinat&aacute;rios e valores constantes da listagem anexa.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 3&ordm; Esta Portaria entra em vigor na data de sua publica&ccedil;&atilde;o.</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p style="text-align: center;"><strong>MANUEL FERNANDO PALÁCIOS DA CUNHA E MELO</strong></p>
<p style="text-align: center;">Secret&aacute;rio da Educa&ccedil;&atilde;o B&aacute;sica</p>';
		
		$textoSQL = simec_htmlspecialchars($texto, ENT_QUOTES);
		
		$sql = "INSERT INTO proinfantil.lote(usucpf, lotnumportaria, lotdataportaria, lotminutaportaria) 
				VALUES ('".$_SESSION['usucpf']."', {$lotnumportaria}, '".formata_data_sql($lotdataportaria)."', '{$textoSQL}') RETURNING lotid;";
		$lotid = $db->pegaUm($sql);
				
		foreach( $cpmid as $id ){
			$sql = "UPDATE proinfantil.mdsdadoscriancapormunicipio SET lotid = $lotid WHERE cpmid = $id";
			$db->executar($sql);
			wf_alterarEstado( $docid[$id], AEDID_MDS_ENCAMINHAR_AGUARDANDO_PAGAMENTO, 'Tramitação em Lote', array( ) );
		}
		
		$html = $texto.'
		<p style="page-break-before:always"><!-- pagebreak --></p>
		<table align="center" class="listagem" border="1" width="100%" cellSpacing="1" cellPadding=3 >
			<tr>
				<th colspan="8" style="text-align: center;">ANEXO</th>
			</tr>
			<tr>
				<th rowspan="2" width="05%"><b>UF</b></th>
				<th rowspan="2" width="25%" style="text-align: center;"><b>Municípios</b></th>
				<th rowspan="2" width="05%" style="text-align: center;"><b>Código IBGE</b></th>
				<th colspan="4" width="60%" style="text-align: justify;"><b>Quantidade de crianças de 0 a 48 meses de famílias beneficiárias do Programa  Bolsa Família, atendidas em creches, declaradas pelos Municípios e o Distrito Federal</b></th>
				<th rowspan="2" width="05%" style="text-align: center;"><b>Valor do Repasse</b></th>
			</tr>
			<tr>
				<th style="text-align: center;"><b>Creche Pública Parcial</b></th>
				<th style="text-align: center;"><b>Creche Pública Integral</b></th>
				<th style="text-align: center;"><b>Creche Conveniada Parcial</b></th>
				<th style="text-align: center;"><b>Creche Conveniada Integral</b></th>
			</tr>';
					
		$arrReferencia = pegaValorReferencia();
		//$arrReferencia[TipoTurma][TipoAtendimentoTurma][TipoRede]
		if($_SESSION['exercicio'] == '2013'){ 
			$valor_creche_pub_int = $arrReferencia[1][1][1];
			$valor_creche_pub_par = $arrReferencia[1][2][1];
			$valor_creche_con_int = $arrReferencia[1][1][2];
			$valor_creche_con_par = $arrReferencia[1][2][2];
			
		} elseif($_SESSION['exercicio'] == '2012'){
			$valor_creche_pub_int = $arrReferencia[1][1][1];
			$valor_creche_pub_par = $arrReferencia[1][2][1];
			$valor_creche_con_int = $arrReferencia[1][1][2];
			$valor_creche_con_par = $arrReferencia[1][2][2];	
		}
			
		$lote = $lotid;
		
		$sql = "select distinct 
					mun.estuf,
				    mun.mundescricao,
				    mun.muncod 
				from proinfantil.mdsdadoscriancapormunicipio mcm
					inner join territorios.municipio mun on mun.muncod = mcm.muncod
				where mcm.lotid = $lotid
				order by mun.estuf, mun.mundescricao";
		
		$arrMunicipio = $db->carregar($sql);
		$arrMunicipio = $arrMunicipio ? $arrMunicipio : array();
		
		foreach ($arrMunicipio as $v) {

			$muncod = $v['muncod'];
			
			$sql = "-- ESCOLAS PÚBLICAS 
			        SELECT   	coalesce(sum( sup.mdsquantidadepbfparcial ),0) as crechepublicaparcial,  
			        			coalesce(sum( sup.mdsquantidadepbfintegral ),0) as crechepublicaintegral
			        FROM 		entidade.entidade ent
			        INNER JOIN 	entidade.endereco ede on ede.entid = ent.entid
			        INNER JOIN 	territorios.municipio mun on mun.muncod = ede.muncod
			        INNER JOIN 	proinfantil.procenso pro on pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
			        INNER JOIN 	proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'
			        WHERE      	mun.muncod = '{$muncod}'
			        AND        	ent.tpcid in (1,2,3)";
			
			$totalMunicipal = $db->pegaLinha($sql);

			$sql = "-- ESCOLAS PRIVADAS  
			        SELECT     	coalesce(sum( sup.mdsquantidadepbfparcial ),0) as crecheconveniadaparcial,
			        			coalesce(sum( sup.mdsquantidadepbfintegral ),0) as crecheconveniadaintegral
			        FROM 		entidade.entidade ent
			        INNER JOIN 	entidade.endereco ede on ede.entid = ent.entid
			        INNER JOIN 	territorios.municipio mun on mun.muncod = ede.muncod
			        INNER JOIN 	proinfantil.procenso pro on pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
			        INNER JOIN 	proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'
			        WHERE       mun.muncod = '{$muncod}'
					AND 		ent.tpcid in (4)";
			
			$totalPrivada = $db->pegaLinha($sql);

			$valorTotal = ($totalMunicipal['crechepublicaparcial']*$valor_creche_pub_par)+($totalPrivada['crecheconveniadaparcial']*$valor_creche_con_par)+($totalMunicipal['crechepublicaintegral']*$valor_creche_pub_int)+($totalPrivada['crecheconveniadaintegral']*$valor_creche_con_int);
			$valorTotal = ($valorTotal ? number_format($valorTotal,2,",",".") : '0,00');
			
			$html.='<tr>
						<td>'.$v['estuf'].'</td>
						<td style="text-align: left;">'.$v['mundescricao'].'</td>
						<td style="text-align: center;">'.$muncod.'</td>
						<td style="text-align: center;">'.$totalMunicipal['crechepublicaparcial'].'</td>
						<td style="text-align: center;">'.$totalMunicipal['crechepublicaintegral'].'</td>
						<td style="text-align: center;">'.$totalPrivada['crecheconveniadaparcial'].'</td>
						<td style="text-align: center;">'.$totalPrivada['crecheconveniadaintegral'].'</td>
						<td style="text-align: right;">'.$valorTotal.'</td>
					</tr>';
			
			$valorTotal = str_replace(".","", $valorTotal);
			$valorTotal = str_replace(",",".", $valorTotal);
			
			$sql = "INSERT INTO proinfantil.loteminutamds(lotid, estuf, muncod, crechepublicaparcial, crechepublicaintegral, crecheconveniadaparcial, crecheconveniadaintegral, valorrepasse) 
					VALUES ({$lotid}, '{$v['estuf']}', '{$muncod}', ".(int)$totalMunicipal['crechepublicaparcial'].", ".(int)$totalMunicipal['crechepublicaintegral'].", 
								".(int)$totalPrivada['crecheconveniadaparcial'].", ".(int)$totalPrivada['crecheconveniadaintegral'].", ".$valorTotal.")";
			$db->executar($sql);
		}
		
		$html.= '</table>';
		
		include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
		ob_clean();
			
		$nomeArquivo 		= 'minuta_repasse_mds_'.date('Y-m-d').'_lote_'.$lote;
		$diretorio		 	= APPRAIZ . 'arquivos/proinfantil/minutaproinfantil';
		$diretorioArquivo 	= APPRAIZ . 'arquivos/proinfantil/minutaproinfantil/'.$nomeArquivo.'.pdf';
		
		if( !is_dir($diretorio) ){
			mkdir($diretorio, 0777);
		}
		
		$http = new RequestHttp();
		$html = utf8_encode($html);
		$response = $http->toPdf( $html );
	
		$fp = fopen($diretorioArquivo, "w");
		if ($fp) {
		  stream_set_write_buffer($fp, 0);
		  fwrite($fp, $response);
		  fclose($fp);
		}
		
		$sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
				VALUES( '".$nomeArquivo."',
						'pdf',
						'".$nomeArquivo."',
						'application/pdf',
						'".filesize($diretorioArquivo)."',
						'".date('Y-m-d')."',
						'".date('H:i:s')."',
						'".$_SESSION["usucpf"]."',
						{$_SESSION['sisid']},
						'A') RETURNING arqid";
		
		$arqid = $db->pegaUm($sql);

		$sql = "UPDATE proinfantil.lote SET arqid = $arqid, lotdsc = 'Lote: '||lotid WHERE lotid = $lotid";
		$db->executar($sql);
	}
	$db->commit();
	echo "<script>
			alert('Lote criado com sucesso.');
			window.location = 'proinfantil.php?modulo=suplementacaomds/pagamentoLote&acao=A';
		  </script>";
}

function confirmaPagamentoLoteMDS( $request ){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	extract($request);
	
	$sql = "SELECT DISTINCT
				dcm.docid
			FROM 
				proinfantil.mdsdadoscriancapormunicipio dcm 
			INNER JOIN workflow.documento doc ON doc.docid = dcm.docid AND doc.esdid = ".WF_MDS_AGUARDANDO_PAGAMENTO."	
			WHERE
				lotid = $lotid and cpmano = ({$_SESSION['exercicio']} - 1)";
	$docids = $db->carregarColuna($sql);
	
	if( $docids[0] != '' ){
		foreach( $docids as $docid ){
			wf_alterarEstado( $docid, AEDID_MDS_ENCAMINHAR_PAGAMENTO_EFETUADO, 'Tramitação em Lote - Confirmar Pagamento', array( ) );
		}
	}
	echo "<script>
			alert('Pagamento confirmado.');
			window.location = 'proinfantil.php?modulo=suplementacaomds/pagamentoLote&acao=A';
		  </script>";
}

function confirmaPagamentoMunicipioMDS( $request ){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	extract($request);
	
	if( $docid[0] != '' ){
		foreach( $docid as $id ){
			wf_alterarEstado( $id, AEDID_MDS_ENCAMINHAR_PAGAMENTO_EFETUADO, 'Tramitação em Lote - Confirmar Pagamento', array( ) );
		}
	}
	echo "<script>
			alert('Pagamento confirmado.');
			window.location = 'proinfantil.php?modulo=suplementacaomds/pagamentoLote&acao=A';
		  </script>";
}
//FIM Funções Aba Pagamento por lote

function verificaAcessoPerfil($usuario,$estworkflow){
	$perfil = pegaPerfil($_SESSION['usucpf']);
	
	$acesso = array("estab" => "N", "enviar_analise" => "N","analise" => "N");
	
	if( in_array(PERFIL_SUPER_USUARIO,$perfil)){
		$acesso = array("estab" => "S", "enviar_analise" => "S","analise" => "S");
	}
	
	if(in_array(PERFIL_ADMINISTRADOR,$perfil) && ($estworkflow == WF_MDS_EM_ANALISE || $estworkflow == WF_MDS_ANALISADO || $estworkflow == WF_MDS_AGUARDANDO_PAGAMENTO)){
		$acesso = array("estab" => "N", "enviar_analise" => "N","analise" => "S");
	}
	
	if(in_array(EQUIPE_MUNICIPAL,$perfil) || in_array(SECRETARIO_ESTADUAL,$perfil) && ($estworkflow == WF_MDS_EM_CADASTRAMENTO || $estworkflow == WF_MDS_EM_DIGILENCIA)){
		$acesso = array("estab" => "S", "enviar_analise" => "S","analise" => "N");
	}
	
	if(in_array(PERFIL_ANALISTA,$perfil) && ($estworkflow == WF_MDS_EM_ANALISE || $estworkflow == WF_MDS_ANALISADO)){
		$acesso = array("estab" => "N", "enviar_analise" => "N","analise" => "S");
	}
	
	if(in_array(PERFIL_COORDENADOR,$perfil) && ($estworkflow == WF_MDS_EM_ANALISE || $estworkflow == WF_MDS_ANALISADO)){
		$acesso = array("estab" => "N", "enviar_analise" => "N","analise" => "S");
	}
	
	if(in_array(PERFIL_ANALISTA_PAGAMENTO,$perfil)){
		$acesso = array("estab" => "N", "enviar_analise" => "N","analise" => "N");
	}
	
	if(in_array(CONSULTA_GERAL,$perfil)){
		$acesso = array("estab" => "N", "enviar_analise" => "N","analise" => "N");
	}
	
	if( $estworkflow == WF_MDS_EM_DIGILENCIA && ($_SESSION['exercicio'] == '2012' && $_SESSION['proinfantil']['mds']['muncod'] != '2211001') ){
		$acesso = array("estab" => "N", "enviar_analise" => "N","analise" => "N");
	}
	
	return $acesso;
}

function pegaValorReferencia(){
	global $db;
	
	$sql = "SELECT vrmvalor, ttuid, tatid, tirid FROM proinfantil.valorreferenciamds WHERE vrmstatus = 'A' and vrmano = '{$_SESSION['exercicio']}'";
	$arrValorRef = $db->carregar($sql);
	$arrValorRef = $arrValorRef ? $arrValorRef : array();
	
	$arrReferencia = array();
	foreach ($arrValorRef as $v) {
		/**
		 * ttuid - Tipo Turma
		 * tatid - Tipo atendimento Turma
		 * tirid - Tipo Rede
		 */
		$arrReferencia[$v['ttuid']][$v['tatid']][$v['tirid']] = $v['vrmvalor'];
	}
	return $arrReferencia;	
}

function verificaEnvioArquivado(){
	if( $_SESSION['exercicio'] == '2012') return true;
	else return false;
}
?>