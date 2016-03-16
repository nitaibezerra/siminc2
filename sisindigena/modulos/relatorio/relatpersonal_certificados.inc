<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de Certificação</b></p>

<? 
	$sql = "select 
uu.unisigla||' - '||uu.uninome as universidade,
p.pfldsc, 
case when c.ceropcao='A' then 'Concluído e certificado' 
     when c.ceropcao='N' then 'Concluído e não certificado'
     else 'Ainda continua cursando' end as situacao,
count(*) as total
from sisindigena.identificacaousuario i 
inner join sisindigena.tipoperfil t on t.iusd = i.iusd 
inner join seguranca.perfil p on p.pflcod = t.pflcod 
inner join sisindigena.universidadecadastro uc on uc.uncid = i.uncid 
inner join sisindigena.universidade uu on uu.uniid = uc.uniid
left join sisindigena.certificados c on c.iusd = i.iusd 
where i.iusstatus='A' and t.pflcod in(1029,1027) 
group by uu.unisigla, uu.uninome, p.pfldsc, c.ceropcao 
order by 1,2,3";
	
	$cabecalho = array("IES","Perfil","Situação","Total");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
?>