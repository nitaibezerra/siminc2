<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

if($_POST['consultarPrefeito']){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = "SELECT
				ende.muncod,
			    ende.estuf,
			    oi.obrid
			FROM
				obras.obrainfraestrutura oi
			    inner join entidade.endereco ende on ende.endid = oi.endid
			WHERE
				oi.orgid = 3
				and oi.obsstatus = 'A'";
	$arrObras = $db->carregar($sql);
	$arrObras = $arrObras ? $arrObras : array();
	
	$strTotalAtualizado = 0;
	$strTotalNaoAtualizado = 0;
	foreach ($arrObras as $vObras) {
		$sql_pref = "SELECT
						max(ent.entid) as entid1				
					FROM entidade.entidade ent
						INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 1 AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
					WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
					and eed2.muncod = '".$vObras['muncod']."' and eed2.estuf = '".$vObras['estuf']."'";
			
		$entPref = $db->pegaUm( $sql_pref );
	
		if( $entPref ){
			$sql = "SELECT DISTINCT
						ent2.entid
					FROM
						entidade.entidade ent
					    left join entidade.funentassoc assoc ON ent.entid = assoc.entid
						left join entidade.funcaoentidade func ON func.fueid = assoc.fueid
						left join entidade.entidade ent2
						    inner join entidade.funcaoentidade func2 ON func2.entid = ent2.entid and func2.funid in (2)
			                inner join entidade.endereco ende on ende.endid = ent2.entid
		                ON ent2.entid =  func.entid
					WHERE
						ent.entid = $entPref";
			
			$entidPrefeito = $db->pegaUm( $sql );
		}
		
		if($entidPrefeito){
			
			$db->executar("UPDATE obras.responsavelcontatos SET recostatus = 'I' WHERE tprcid = 8 and recoid in (SELECT rc.recoid FROM
																									obras.responsavelcontatos rc
																								    inner join obras.responsavelobra ro on ro.recoid = rc.recoid
																								WHERE
																									rc.tprcid = 8
																									and ro.obrid =".$vObras['obrid'].")");
			
			$db->executar("DELETE FROM obras.responsavelobra WHERE recoid in (SELECT rc.recoid FROM
																									obras.responsavelcontatos rc
																								    inner join obras.responsavelobra ro on ro.recoid = rc.recoid
																								WHERE
																									rc.tprcid = 8
																									and ro.obrid =".$vObras['obrid'].")");
			
			$sql = "INSERT INTO obras.responsavelcontatos(entid, tprcid) VALUES (".$entidPrefeito.", 8) returning recoid";
			$recoid = $db->pegaUm($sql);
			$sql = "INSERT INTO obras.responsavelobra(recoid, obrid) VALUES ($recoid, ".$vObras['obrid'].")";
			$db->executar($sql);
			if($db->commit()){ 
				$strTotalAtualizado++;
			} else {
				$strTotalNaoAtualizado++;
			}
		} else {
			$strTotalNaoAtualizado++;
		}
	}

	$html = '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulocentro" colspan="2">Operação realizada com sucesso!</br></td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="33%;">Total de Obras Carregadas:</td>
					<td>'.sizeof($arrObras).'</td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="33px;">Total de Obras Atualizadas:</td>
					<td>'.$strTotalAtualizado.'</td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="33px;">Total de Obras Não Atualizadas:</td>
					<td>'.$strTotalNaoAtualizado.'</td>
				</tr>
			</table>';
	echo $html;
	exit();
}

// Cabeçalho padrão do sistema
include APPRAIZ . 'includes/cabecalho.inc';

// monta o título da tela
print '<br/>';
?>
<div id="divFormulario"></div>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script type="text/javascript" language="javascript">
	//var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
	var url = "/obras/obras.php?modulo=principal/carga_prefeito&acao=A";
	var pars = 'consultarPrefeito=1';

	//var bkDiv = $('divFormulario').innerHTML;
	
	var myAjax = new Ajax.Request(
			url, 
			{
				method: 'post', 
				parameters: pars, 
				asynchronous: false,
				//onLoading: $('divFormulario').update(carregando+' Aguarde enquanto sua solicitação é finalizada. '),
				onComplete: function(r)
				{
					//$('divDebug').update(r.responseText);
					var erro = trim(r.responseText.substring(0,6));
					var tamanho = r.responseText.length;
					if(erro == 'Erro-' ){
						var resultado = trim(r.responseText.substring(6,tamanho));
						$('divFormulario').update(resultado);
						$('divFormulario').style.color = 'red';								
						$('divFormulario').style.fontSize = '14px';								
						$('divFormulario').style.textAlign = 'center';
					} else {
						$('divFormulario').update(r.responseText);
					}
					//return false
				}
			});
</script>