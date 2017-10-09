<?php
ob_end_clean();
//Chamando a classe de FPDF
require('../../includes/fpdf/fpdf.inc');
include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . 'includes/funcoes.inc';
$data = new Data();

$prtid = $_SESSION["academico"]["prtid"];

if($prtid)
{
	$q = "
			SELECT 
				prtnumero,
				tprid,
				prtdtportaria
			FROM
				academico.portarias
			WHERE
				prtid = $prtid
	";
	$dadosPortaria = $db->carregar( $q );
}
else
{
	echo '<script type="text/javascript"> 
		 			alert( "Faltam parâmetros para gerar a Portaria." ); 
			 		window.close(); 
			 	 </script>';  
}

$PDF = new FPDF( 'P','cm','A4' ); //documento em formato de retrato, medido em cm e no tamanho A4
$PDF -> SetMargins(1, 1, 1); // margem esquerda = 3 , superior = 3 e direita = 2.
$PDF -> SetAuthor(SIGLA_SISTEMA); //informando o autor do documento.
$PDF -> SetTitle(SIGLA_SISTEMA); //informando o título do documento.
$PDF -> AddPage(); //adicionando um nova página

$PDF->ln(6); //quebra de linha

//$formTst = date("d/m/Y", $dadosPortaria[0]['prtdtpublicacaomec']);
$anoPortaria = $data->formataData($dadosPortaria[0]['prtdtportaria'], "YYYY");
$formatDate = $data->formataData($dadosPortaria[0]['prtdtportaria'], "           de mesTextual de YYYY");

		$PDF->SetFont('Times', 'B', 11); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF->Cell(19,1,'PORTARIA Nº                           MEC,            DE                       DE',0,1,'C',0);
		$PDF->SetFont('Times', '', 10); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$sql = "
			SELECT 
				cp.copid as id_reg,
				cp.prtid as id,
				cp.copordem as ordem, 
				tp.tpcid as idtipo, 
				tp.tpcdesc as tipo, 
				cp.copdesc as conteudo 
			FROM academico.composicaoportaria cp INNER JOIN  academico.tipocomposicao tp ON cp.tpcid = tp.tpcid
			WHERE prtid=".$prtid." AND tp.tpcid IN (1,4,5)
			ORDER BY cp.copordem";
		//dbg($sql,1);
			$dados=$db->carregar( $sql );
		
		$cont_art=1;	
		for($i = 0; $i < count($dados); $i++) {
			$tipo=$dados[$i]['tipo'];
			$idtipo=$dados[$i]['idtipo'];
			$id_registro=$dados[$i]['id_reg'];
			$conteudo=$dados[$i]['conteudo'];
			// tabulacao
			$mostrar="                         ";
			
			if ($idtipo==1){
				$mostrar.=$tipo." ".$cont_art." ° ";
				$mostrar.= $conteudo;
				$cont_art++;
				// imprime na tela
				$PDF->MultiCell(19,0.5,$mostrar,0,1,'J',0);
				$PDF->SetX(3.5);
				$PDF->ln();
				//Incisos e paragrafos dos artigos
				$sql2 = "SELECT 
								tp.tpcid as idtipo, 
								tp.tpcdesc as tipo, 
								cp.copdesc as conteudo 
						FROM academico.composicaoportaria cp INNER JOIN  academico.tipocomposicao tp ON cp.tpcid = tp.tpcid
						WHERE cp.copidinciso= $id_registro
						ORDER BY tp.tpcid";
						$dados2=$db->carregar( $sql2 );
						
						$cont_parag=1;
						$cont_inci=1;
						
						for($h = 0; $h < count($dados2); $h++) {
							$idtipo2=$dados2[$h]['idtipo'];
							$conteudo2 = $dados2[$h]['conteudo'];
							$mostrar2="                         ";
							
							// Parágrafo
							if ($idtipo2==3){
								$mostrar2.=" § $cont_parag ° ".$conteudo2;
								$cont_parag++;
								// imprime na tela
								$PDF->MultiCell(19,0.5,$mostrar2,0,1,'J',0);
								$PDF->SetX(3.5);
								$PDF->ln();
								
							} 
								// Inciso
							if ($idtipo2==2){
								$mostrar2.= decimal2romano($cont_inci)." ".$conteudo2;
								$cont_inci++;
								// imprime na tela
								$PDF->MultiCell(19,0.5,$mostrar2,0,1,'J',0);
								$PDF->SetX(3.5);
								$PDF->ln();
							} 
							
								// Parágrafo Unico de Artigo
							if ($idtipo2==6){
								$mostrar2.= "Parágrafo único. ".$conteudo2;
								
								// imprime na tela
								$PDF->MultiCell(19,0.5,$mostrar2,0,1,'J',0);
								$PDF->SetX(3.5);
								$PDF->ln();
							} 
							
						}
				//Fim Incisos e paragrafos dos artigos
				
				
			}
			else {
				if ($idtipo==5){
					$mostrar.="Parágrafo único. ";
					$mostrar.= $conteudo;
					// imprime na tela
					$PDF->MultiCell(19,0.5,$mostrar,0,1,'J',0);
					$PDF->SetX(3.5);
					$PDF->ln();
				}
				if ($idtipo==4){
					$mostrar.= $conteudo;
					// imprime na tela
					$PDF->MultiCell(19,0.5,$mostrar,0,1,'J',0);
					$PDF->SetX(3.5);
					$PDF->ln();
				}
			}
					
		}

		$PDF->ln(1);
		$PDF->SetX(3.5);

		$PDF->SetFont('Arial', 'B', 10); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF->ln(1);
		$assina=$db->pegaUm("select a.nasdsc from academico.portarias p INNER JOIN academico.assinatura a ON p.nasid = a.nasid WHERE prtid=".$_SESSION["academico"]["prtid"]);
		$PDF->Cell(19,1,$assina,0,1,'C',0);
		 
		$PDF -> AddPage(); //adicionando um nova página
		$PDF->Cell(19,1,'Anexo I',0,1,'C',0);
			    
		$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF ->SetFillColor(156, 156, 156); //cor de fundo da tabela
		$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF->Cell(5,0.5,'Código da Instituicao',0,0,'C',1);
		$PDF->Cell(14,0.5,'Instituição de Ensino',0,1,'C',1);
		$PDF->ln(0.2);
	
		//Gerando a tabela com os dados por Campus
		$sql = "
			SELECT 
				uni.entnome as universidade,
				uni.entid as  entidentidade,
				uni.entunicod
            FROM 
				academico.portarias AS prt 
				INNER JOIN academico.entidadeportaria enp ON enp.prtid=prt.prtid
				INNER JOIN entidade.entidade AS uni ON uni.entid = enp.entid 
			WHERE prt.prtid = $prtid 
			ORDER BY uni.entnome "; 
		$dadosUniversidade = $db->carregar( $sql );
		
		//dbg($sql);
		
		if ($dadosUniversidade){	

			for( $i = 0; $i < count( $dadosUniversidade); $i++)
			{
			$id_entidade=$dadosUniversidade[$i]['entidentidade'];

				$sqlCampus = "
						SELECT 
							DISTINCT(cam.entid) as entid,						   
							cam.entnome as campus  
						FROM 
						    academico.lancamentosportaria AS lc 
						    INNER JOIN entidade.entidade AS cam ON cam.entid = lc.entidcampus
						WHERE lc.lnpstatus ='A' and lc.prtid=$prtid
						    AND lc.entidentidade=$id_entidade";
		        //dbg($sqlCampus,1);
				$dadosCampus = $db->carregar( $sqlCampus );
				if ($dadosCampus){
					$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
					$PDF ->SetFillColor(220, 220, 220); //cor de fundo da tabela
					
					$PDF->Cell(5,0.5,$dadosUniversidade[$i]['entunicod'],0,0,'C',1);
					$PDF->Cell(14,0.5,$dadosUniversidade[$i]['universidade'],0,1,'C',1);
					$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
					
					$PDF->Cell(5,0.5,'',1,0,'C',1);
					$PDF->Cell(14,0.5,'Autorizações',1,1,'C',1);
					
					$PDF->Cell(5,0.3,'Campus ',1,0,'C',0);
					for( $k = 0; $k<count($dadosCampus); $k++)
					{
						$campus=$dadosCampus[$k]['campus'];	
						$id_campus=$dadosCampus[$k]['entid'];
						
						
						$sql = "
							SELECT 
							    cls.clsdsc as classe,
							    CASE WHEN lc.lnpvalor is not null THEN
							    lc.lnpvalor
							    ELSE 
							    0   
							    END as lancamento,
							    cls.clsid
							    
							FROM 
							    academico.lancamentosportaria AS lc 
							    INNER JOIN entidade.entidade AS cam ON cam.entid = lc.entidcampus
							    INNER JOIN academico.classes as cls ON cls.clsid = lc.clsid
							WHERE lc.lnpstatus ='A' and lc.prtid=$prtid 
							    AND lc.entidcampus=$id_campus
							ORDER BY cam.entid
						";
						//dbg($sql);
						$dadosClasses = $db->carregar( $sql );
						//dbg($dadosClasses,1);
						
						for( $h = 0; $h<count($dadosClasses); $h++)
						{
							if($dadosClasses[$h]['classe']=="Docentes")
								$classe=$dadosClasses[$h]['classe'];
							else
								$classe="Nível ".$dadosClasses[$h]['classe'];
								$PDF->SetFillColor(234, 234, 234); //cor de fundo da tabela
								if ($h<6 and $k==0)
									$PDF->Cell(2.8,0.3,$classe,1,0,'C',1);
						}
						
						$PDF->ln(0.3);
						$PDF->Cell(5,0.3,$campus,1,0,'L',0);
						
						for( $h = 0; $h<count($dadosClasses); $h++)
						{
							$lancamento=$dadosClasses[$h]['lancamento'];
							$PDF->Cell(2.8,0.3,$lancamento,1,0,'C',0);
							
						}
					}	
				}
				$PDF->ln(0.3);
				
			}	 
		}

	    if (trim($dadosPortaria[0]['prtnumero']) <>'')
			$arqnome = 'Portaria_'.trim($dadosPortaria[0]['prtnumero']).'_'.$anoPortaria;
		else
			$arqnome = 'Portaria_'.trim($prtid).'_'.$anoPortaria;
	    //Insere o registro do arquivo na tabela public.arquivo
		$sql = "INSERT INTO public.arquivo 	
				(
					arqnome,
					arqextensao,
					arqdescricao,
					arqtipo,
					arqtamanho,
					arqdata,
					arqhora,
					usucpf,
					sisid
				)VALUES(
					'$arqnome',
					'pdf',
					'Portaria',
					'application/pdf',
					'0',
					'".date('Y-m-d')."',
					'".date('H:i:s')."',
					'".$_SESSION["usucpf"]."',
					". $_SESSION["sisid"] ."
				) RETURNING arqid;";
		$arqid = $db->pegaUm($sql);
		//Insere o arqid na tabela sig.notatecnica
		$verifica = "SELECT anxid FROM academico.anexos WHERE arqid = $arqid";
		$existe = $db->pegaUm( $verifica );
		if( !$existe )
		{
			$sql = "INSERT INTO academico.anexos (arqid, prtid, tpaid, anxdesc, anxstatus, anxdtinclusao)
					VALUES
					(
						$arqid,
						$prtid,
						13,
						'{$arqnome}',
						'A',
						'now'
					)
			";
		}
		else
		{
			$sql = "UPDATE academico.anexos
					SET arqid = '$arqid'
						WHERE prtid = $prtid";
		}
		$db->executar($sql);
		$db->commit();
		
		ob_end_clean();
		if(!is_dir('../../arquivos')) {
		mkdir(APPRAIZ.'/arquivos', 0777);
		}
		if(!is_dir('../../arquivos/academico')) {
			mkdir(APPRAIZ.'/arquivos/academico', 0777);
		}
		if(!is_dir('../../arquivos/academico/'.floor($arqid/1000))) {
			mkdir(APPRAIZ.'/arquivos/academico/'.floor($arqid/1000), 0777);
		}
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'.$arqid;
		$PDF->Output($caminho,'F'); //saída do PDF
		//echo "<script>window.opener.location.reload(); window.close();</script>";
		$PDF->Output();
?>
