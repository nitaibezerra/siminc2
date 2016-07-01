<?php
class EmendaParlamentar {
	
	public function __construct(){
		
	}
	
	public function sqlListaAEmpenhar($obProcesso){
		
		$sql = "select 
					'<center><input type=checkbox name=chk[] onclick=marcarChk(this); id=chk_'||ptrid||' value='||ptrid||'>
							<a href=\"javascript:mostrarDadosPTA(\'' || ptrid || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar PTA\" border=\"0\"></a>
						</center>' as chk,
				    pta,
				    valorpta,
				    anoemenda,
				    numemenda,
				    valoremenda,
				    porcentoempenhado,
				    valrempenhado,
				    '<input type=text id=id_'||ptrid||' value='||porcentoempenhar||' name=name_'||ptrid||' size=6 onKeyUp=\"calculaEmpenho(this);\" onBlur=\"verificaPreenchimentoPorcentagem(this)\" class=\"disabled\" readonly=\"readonly\" onfocus=\"this.select();\">'|| 
				    '<input type=hidden id=vlr_'||ptrid||' name=vlr_'||ptrid||' value='||porcentoempenhar||'>'||
				    '<input type=hidden id=ano_'||ptrid||' name=ano_'||ptrid||' value='||anoemenda||'>' as porcentoaempenhar,
					'<input type=\"text\" style=\"text-align:right;\" onkeyup=\"calculaEmpenho(this);\" class=\"normal\" id=\"id_vlr_'||ptrid||'\" value=\"'||vlrempenhar||'\" name=\"name_vlr_'||ptrid||'\"' as vlrempenhar
				    
				 from (
				SELECT DISTINCT
                    ptr.ptrid,
                    ptr.ptrcod||'&nbsp;' as pta,
				    to_char(pti.ptivalortotal, '999G999G999D99') as valorpta,
				    vede.emeano ||'&nbsp;' as anoemenda,
				    vede.emecod||'&nbsp;' as numemenda,
				    to_char(vede.emdvalor, '999G999G999D99') as valoremenda,
                    --case when sum(exf.exfvalor) is not null then (pti.ptivalortotal*100)/sum(exf.exfvalor) else '100' end as porcentoempenhar,
                    --case when sum(exf.exfvalor) is not null then (pti.ptivalortotal*100)/sum(exf.exfvalor) else '100' end as porcentoempenhar,
                    '0' as porcentoempenhado,
                    to_char(0, '999G999G999D99') as valrempenhado,
                    '100' as porcentoempenhar,
                    to_char(pti.ptivalortotal, '999G999G999D99') as vlrempenhar
				FROM
					emenda.planotrabalho ptr
				    inner join
				      (SELECT 
				          ptrid,
				          sum(ptivalortotal) as ptivalortotal
				       FROM emenda.v_ptiniciativa 
				       GROUP BY ptrid) pti ON pti.ptrid = ptr.ptrid
				    inner join emenda.ptemendadetalheentidade pte on pte.ptrid = ptr.ptrid
				    inner join emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid
				    inner join execucaofinanceira.processos pro on pro.pronumeroprocesso = ptr.ptrnumprocessoempenho
                    --left join emenda.execucaofinanceira exf on exf.ptrid = ptr.ptrid and exf.pedid = pte.pedid and exf.exfstatus = 'A'
				WHERE
					ptr.ptrnumprocessoempenho = '{$obProcesso->pronumeroprocesso}'
					and ptr.ptrstatus = 'A'
				    and vede.edestatus = 'A'
			) as foo";
		
		return $sql;
	}
}
?>