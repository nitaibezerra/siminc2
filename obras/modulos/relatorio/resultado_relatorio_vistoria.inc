<?php

	#Salva os POST na tabela
	if ( $_REQUEST['salvar'] == 1 ){
		$existe_rel = 0;
		$sql = sprintf(
				"select prtid from public.parametros_tela where prtdsc = '%s'",
				$_REQUEST['titulo']
		);
		$existe_rel = $db->pegaUm( $sql );
		if ($existe_rel > 0){
			$sql = sprintf(
					"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
					$_REQUEST['titulo'],
					addslashes( addslashes( serialize( $_REQUEST ) ) ),
					$_SESSION['usucpf'],
					$_SESSION['mnuid'],
					$existe_rel
			);
			$db->executar( $sql );
			$db->commit();
		}else{
			$sql = sprintf(
					"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
					$_REQUEST['titulo'],
					addslashes( addslashes( serialize( $_REQUEST ) ) ),
					'FALSE',
					$_SESSION['usucpf'],
					$_SESSION['mnuid']
			);
			$db->executar( $sql );
			$db->commit();
		}
		echo "<script type=\"text/javascript\">";
			echo "alert('Operação realizada com sucesso!');";
			echo "location.href = '?modulo".$modulo."&acao=A';";
		echo "</script>";
		die();
	}

	if ($_REQUEST['filtrosession']){
		$filtroSession = $_REQUEST['filtrosession'];
	}
	
	if ($_POST['agrupador']){
		header('Content-Type: text/html; charset=iso-8859-1');
	}
	
	#Configurações do relatorio - Memoria limite de 1024 Mbytes
	ini_set("memory_limit", "1024M");
	set_time_limit(0);
	#FIM configurações - Memoria limite de 1024 Mbytes
?>	
	<html>
	<head>
	<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
		
<?php
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	$sql   = monta_sql();
	$dados = $db->carregar($sql);
	$agrup = monta_agp();
	$col   = monta_coluna();
	
	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados); 
	$r->setColuna($col);
	$r->setBrasao(true);
	$r->setTotNivel(true);
	$r->setEspandir(false);
	$r->setMonstrarTolizadorNivel(true);
	$r->setTotalizador(true);
	$r->setTolizadorLinha(true);

	if($_POST['tipo_relatorio'] == 'xls'){
		ob_clean();
		$nomeDoArquivoXls="relatorio_vistoria_".date('d-m-Y_H_i');
		echo $r->getRelatorioXls();
	}elseif($_POST['tipo_relatorio'] == 'visual'){
		echo $r->getRelatorio();	
	}
?>	
	
	
<?php	
	function monta_sql(){
		
		$where = array();
		
		extract($_REQUEST);
		
		#Estado - Unidades Federativas.
		if($estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){
			$where[0] = " and ed.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";
		}		
		
		#Município.
		if($municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
			$where[1] = " and ed.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";
		}

		#Período de inclusão da obra.
		if($orgid == 'T'){
			$where[2] = " and o.orgid in (1,2,3) ";		
		}elseif($orgid == 'S'){
			$where[2] = " and o.orgid = 1 ";
		}elseif($orgid == 'P'){
			$where[2] = " and o.orgid = 2";
		}elseif($orgid == 'B'){
			$where[2] = " and o.orgid = 3";
		}

		#Município.
		if($htddata_inicio != '' && $htddata_fim != ''){
			$where[3] = " and mc.mpcdtinclusao Between '". formata_data_sql($htddata_inicio). "' and '". formata_data_sql($htddata_fim) ."'" ;
		}		
		
		
		#Situação do parecer
		if($situacao_parecer == 'T'){
			$where[4] = " and mc.mpcsituacao in ('t', 'f') ";
		}elseif($situacao_parecer == 'A'){
			$where[4] = " and mc.mpcsituacao = 't' ";
		}elseif($situacao_parecer == 'N'){
			$where[4] = " and mc.mpcsituacao = 'f' ";
		}
		
		
		$sql = "
			Select	o.obrid as id_obra,
					ed.estuf as uf_obras,
					o.obrdesc as nome_obra,
					to_char(o.obsdtinclusao, 'dd/mm/yyyy') as data_insercao_obra,
					to_char(ord.orsdtemissao, 'dd/mm/yyyy') as data_emissao_os,
					to_char(max(mc.mpcdtinclusao), 'dd/mm/yyyy') as data_ultima_paracer,
					Case 
					   when mc.mpcsituacao = 't' 
					      Then 'Aprovado'
					      Else 'Não Aprovado'
					end as situacao_parecer,
					su.usunome as supervisor_mec,
					to_char(max(hist.htddata), 'dd/mm/yyyy') as data_tramitacao,
					hist.usunome as res_tramitacao
					
			From obras.obrainfraestrutura o
			
			Join entidade.endereco ed on ed.endid = o.endid
			Join obras.checklistvistoria ck on o.obrid = ck.obrid
			Join obras.movparecercklist mc on ck.chkid = mc.chkid 
			Join seguranca.usuario su on su.usucpf = mc.usucpf
			Join obras.repositorio r on o.obrid  = r.obrid
			Join obras.itemgrupo it on r.repid  = it.repid
			Join obras.grupodistribuicao g on it.gpdid = g.gpdid
			Join obras.ordemservico ord on ord.gpdid = g.gpdid
			
			Join (
				Select	h.docid,
					usu.usunome,
					h.htddata
				From workflow.historicodocumento h
				Join workflow.documento d on d.docid = h.docid
				left Join seguranca.usuario usu on usu.usucpf = h.usucpf
				--Group by h.docid, usu.usunome
			) hist on hist.docid  = g.docid
			
			where o.obsstatus = 'A' and mpcstatus = 'A' and chkstatus = 'A' and ord.orsstatus = 'A'
			
			--Estado
			".$where[0]."
			--Município.
			".$where[1]."
			--Unidade
			".$where[2]."
			--Período
			".$where[3]."
			--Sitação --A aprovado --N Não aprovado
			".$where[4]."
						
			Group by o.obrid, ed.estuf, g.gpdid, o.obrdesc, hist.usunome, situacao_parecer, su.usunome, o.obsdtinclusao, ord.orsdtemissao
			
			order by uf_obras, ord.orsdtemissao, o.obrid
		";
		//ver($sql, d);
		return $sql;
	}
	
	function monta_agp(){
		
		$agp = 	array(	
					"agrupador" => array(),
					"agrupadoColuna" => array(/*"id_obra",*/ "uf_obras", "nome_obra", "data_insercao_obra", "data_emissao_os", "data_ultima_paracer", "situacao_parecer", "supervisor_mec", "data_tramitacao", "res_tramitacao")
		);
		array_push($agp['agrupador'], array("campo" => "id_obra","label" => "ID Obra"));
		
		#Agrupador padrão.
		#array_push($agp['agrupador'], array("campo" => "supvid", "label" => "Vistorias") );
	
		return $agp;	
	}	
	
	function monta_coluna(){
	
		$colunas = $_POST['colunasDest'];
		$colunas = $colunas ? $colunas : array();
	
		$coluna = array();	
		
		foreach ($colunas as $val){
			switch ($val) {
				/*case 'id_obra':
					array_push( $coluna,
						array(	"campo"		=> "id_obra",
								"label" 	=> "ID Obra",
								"type"	  	=> "string"
						)
					);
					continue;
				break;*/
				case 'uf_obras':
					array_push( $coluna,
						array(	"campo" 	=> "uf_obras",
								"label"		=> "Estado",
								"type"	  	=> "string"
						)
					);
					continue;
				break;
				case 'nome_obra':
					array_push( $coluna,
						array(	"campo" 	=> "nome_obra",
								"label"		=> "Nome da Obra",
								"type"	  	=> "string"
						)
					);
					continue;
				break;
				case 'data_insercao_obra':
					array_push( $coluna,
						array(	"campo" 	=> "data_insercao_obra",
								"label"		=> "Data de Inserção da Obra",
								"type"	  	=> "string"
						)
					);
					continue;
				break;
				case 'data_emissao_os':
					array_push( $coluna,
						array(	"campo" 	=> "data_emissao_os",
								"label"		=> "Data da emissão da OS",
								"type"	  	=> "string"
						)
					);
					continue;
				break;
				case 'data_ultima_paracer':
					array_push( $coluna,
						array(	"campo" 	=> "data_ultima_paracer",
								"label"		=> "Data do Parecer",
								"type"	  	=> "string"
						)
					);
					continue;
				break;
				case 'situacao_parecer':
					array_push( $coluna,
						array(	"campo" 	=> "situacao_parecer",
								"label"		=> "Situação do Parecer",
								"type"	  	=> "string"
						)
					);
					continue;
				break;
				case 'supervisor_mec':
					array_push( $coluna,
						array(	"campo" 	=> "supervisor_mec",
								"label"		=> "Supervisor do MEC",
								"type"	  	=> "string"
						)
					);
					continue;
				break;
				case 'data_tramitacao':
					array_push( $coluna,
						array(	"campo" 	=> "data_tramitacao",
								"label"		=> "Data da Tramitação",
								"type"	  	=> "string"
						)
					);
					continue;
				break;
				case 'res_tramitacao':
					array_push( $coluna,
						array(	"campo" 	=> "res_tramitacao",
								"label"		=> "Resp. pela Tramitação",
								"type"	  	=> "string"
						)
					);
					continue;
				break;
			}
		}
		return $coluna;
	}
?>
