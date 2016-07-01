<?php
class Seed extends cls_banco{
	
	public function __construct(){
		parent::__construct();
	}
	
	public function montaCabecalhoProcesso( $prcid ){
		$sql = "SELECT 
				  	tp.tpcdsc,
				  	p.prcnumprocesso,
				  	s.sitdsc
				FROM 
					seed.processo p
				  	inner join seed.tipoprocesso tp
				    	on tp.tpcid = p.tpcid
				    left join seed.situacao s
				    	on s.sitid = p.sitid
				        and s.sitstatus = 'A'
				WHERE
					p.prcstatus = 'A'
				    and tp.tpcstatus = 'A'
				    and p.prcid = $prcid";
		
		$arProcesso = parent::pegaLinha( $sql );
		
		//23000.123456/2010-15
		
		$arProcesso['prcnumprocesso'] = substr($arProcesso['prcnumprocesso'],0,5) . "." .
										substr($arProcesso['prcnumprocesso'],5,6) . "/" .
										substr($arProcesso['prcnumprocesso'],11,4) . "-" .
										substr($arProcesso['prcnumprocesso'],15,2);
		
		echo '
		<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" style="width:39.5%;">Tipo do Processo:</td>
			<td style="width:60.5%;">'.$arProcesso['tpcdsc'].'</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nº Processo:</td>
			<td>'.$arProcesso['prcnumprocesso'].'</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação:</td>
			<td>'.$arProcesso['sitdsc'].'</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2">&nbsp;</td>
		</tr>
		</table>';
	}
	
	public function validaSession($session, $boProcesso = true){
		if( $boProcesso ){
			if( empty($session) ){
				echo "<script>
						alert('Faltam dados na sessão sobre este Processo.');
						window.location.href = 'seed.php?modulo=principal/listaProcessos&acao=A';
					  </script>";
				die();
			}	
		} else {
			if( empty($session) ){
				echo "<script>
						alert('Faltam dados na sessão.');
						history.back(-1);
					  </script>";
				die();
			}
		}
	}
}
?>